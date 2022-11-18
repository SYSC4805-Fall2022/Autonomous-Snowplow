#include "distance_gp2.h"
#include "ir_sensor.h"
#include "line_follower.h"
#include "ultrasonic_sensor.h"
#include "motor_control.h"
#include <Arduino.h>

// Front Line Follower Sensor Pin Out
#define FLFS_R_pin 23
#define FLFS_M_pin 25
#define FLFS_L_pin 27

// Back Line Follower Sensor Pin Out
#define BLFS_R_pin 43
#define BLFS_M_pin 41
#define BLFS_L_pin 39

// Stepper Motors Pin Out
//Back Left
#define BL_Wheel_Enable 11 // 47
#define BL_Wheel_Direction 3
//Back Right
#define BR_Wheel_Enable 9 // 51
#define BR_Wheel_Direction 5
//Front Left
#define FL_Wheel_Enable 10 // 49
#define FL_Wheel_Direction 4
//Front Right
#define FR_Wheel_Enable 8 // 53
#define FR_Wheel_Direction 6

// Front Right IR Sensor
#define FR_Turn_Sensor 36
// Back Left IR Sensor
#define BL_Turn_Sensor 34

// Back GP2 Distance Sensor
#define GP2_Sensor A0

// Ultrasonic Sensor
#define ultrasonic_trig 2
#define ultrasonic_echo A7
EZDist EZDist(ultrasonic_trig, ultrasonic_echo);

// WDT
#define WDT_KEY (0xA5)
volatile int phase = 1;

// States
const int state_forward = 0;
const int state_turn_right = 1;
const int state_backup = 2;
const int state_inch_forward = 3;
const int state_random_turn_right = 4;

const int main_object_detection_threshold = 10;
const int short_object_detection_threshold = 5;
const int max_stuck_counter = 5;

volatile int state = state_forward;
volatile bool line_following = true;
volatile int right_turns = 0;
volatile int stuck_counter = 0;

void watchdogSetup(void) {
  /*** watchdogDisable (); ***/
}

void WDT_Handler(void)
{
  if (phase)
  /* Clear status bit to acknowledge interrupt by dummy read. */
  WDT->WDT_SR; // Clear status register

  printf("help! in WDT\n");
}

void state_forward_handler(){
  bool front_line_detected = front_detection(FLFS_R_pin, FLFS_M_pin, FLFS_L_pin);
  bool front_object_detected = object_detection_ultrasonic(EZDist, main_object_detection_threshold);
  stuck_counter = 0;

  if(line_following){
    int direction = steer_direction(FLFS_R_pin, FLFS_M_pin, FLFS_L_pin);
    // Straight = 1, left = 0, right = 2, corner = 3
    switch (direction){
      case 1:
        forward(BL_Wheel_Direction, BR_Wheel_Direction, FL_Wheel_Direction, FR_Wheel_Direction);
        enable_on(BL_Wheel_Enable, BR_Wheel_Enable, FL_Wheel_Enable, FR_Wheel_Enable);
        break;
      case 0:
        forward(BL_Wheel_Direction, BR_Wheel_Direction, FL_Wheel_Direction, FR_Wheel_Direction);
        enable_left_turn(BL_Wheel_Enable, BR_Wheel_Enable, FL_Wheel_Enable, FR_Wheel_Enable);
        break;
      case 2:
        forward(BL_Wheel_Direction, BR_Wheel_Direction, FL_Wheel_Direction, FR_Wheel_Direction);
        enable_right_turn(BL_Wheel_Enable, BR_Wheel_Enable, FL_Wheel_Enable, FR_Wheel_Enable);
        break;
      case 3:
        enable_off(BL_Wheel_Enable, BR_Wheel_Enable, FL_Wheel_Enable, FR_Wheel_Enable);
        if (steer_direction(FLFS_R_pin, FLFS_M_pin, FLFS_L_pin) == 3){
          state = state_turn_right;
        }
        break;
    }
  }else if(front_line_detected || front_object_detected) {
    enable_off(BL_Wheel_Enable, BR_Wheel_Enable, FL_Wheel_Enable, FR_Wheel_Enable);
    state = state_backup;
  }else{
    forward(BL_Wheel_Direction, BR_Wheel_Direction, FL_Wheel_Direction, FR_Wheel_Direction);
    enable_on(BL_Wheel_Enable, BR_Wheel_Enable, FL_Wheel_Enable, FR_Wheel_Enable);   
  }
}

void state_turn_right_handler(){
  right(BL_Wheel_Direction, BR_Wheel_Direction, FL_Wheel_Direction, FR_Wheel_Direction);
  enable_on(BL_Wheel_Enable, BR_Wheel_Enable, FL_Wheel_Enable, FR_Wheel_Enable);
  
  delay(500);

  enable_off(BL_Wheel_Enable, BR_Wheel_Enable, FL_Wheel_Enable, FR_Wheel_Enable);
  delay(100);
  forward(BL_Wheel_Direction, BR_Wheel_Direction, FL_Wheel_Direction, FR_Wheel_Direction);
  enable_on(BL_Wheel_Enable, BR_Wheel_Enable, FL_Wheel_Enable, FR_Wheel_Enable);
  right_turns += 1;
  if (right_turns == 5){
    line_following = false;
  }
  state = state_forward;
}

void state_backup_handler(){
  enable_off(BL_Wheel_Enable, BR_Wheel_Enable, FL_Wheel_Enable, FR_Wheel_Enable);
  backward(BL_Wheel_Direction, BR_Wheel_Direction, FL_Wheel_Direction, FR_Wheel_Direction);
      
  for (int i = 0; i < 10; i++){
    bool back_line_detected = back_detection(BLFS_R_pin, BLFS_M_pin, BLFS_L_pin);
    bool back_object_detected = object_detection_gp2(GP2_Sensor, main_object_detection_threshold);
  
    if (back_line_detected || back_object_detected){
      enable_off(BL_Wheel_Enable, BR_Wheel_Enable, FL_Wheel_Enable, FR_Wheel_Enable);
      state = state_inch_forward;
      break;
    } else {
      enable_on(BL_Wheel_Enable, BR_Wheel_Enable, FL_Wheel_Enable, FR_Wheel_Enable);
      delay(100);
    }

    if (i == 9){
      state = state_random_turn_right;
    }
  }
}

void state_inch_forward_handler(){
  bool front_line_detected = front_detection(FLFS_R_pin, FLFS_M_pin, FLFS_L_pin);
  bool front_object_detected = object_detection_ultrasonic(EZDist, short_object_detection_threshold);
  
  if(!front_line_detected || !front_object_detected){
    forward(BL_Wheel_Direction, BR_Wheel_Direction, FL_Wheel_Direction, FR_Wheel_Direction);
    enable_on(BL_Wheel_Enable, BR_Wheel_Enable, FL_Wheel_Enable, FR_Wheel_Enable);
    delay(100);
    enable_off(BL_Wheel_Enable, BR_Wheel_Enable, FL_Wheel_Enable, FR_Wheel_Enable);
    state = state_random_turn_right;
  } else if(stuck_counter == max_stuck_counter){
    state = state_random_turn_right; // We are stuck, we should try to get out
  } else {
    state = state_backup;
    stuck_counter += 1;
  }
}

void state_random_turn_right_handler(){
  bool front_line_detected = front_detection(FLFS_R_pin, FLFS_M_pin, FLFS_L_pin);
  bool front_object_detected = object_detection_ultrasonic(EZDist, short_object_detection_threshold);
  bool turn_fr_sensor = turn_check_front_right(FR_Turn_Sensor);
  bool turn_bl_sensor = turn_check_back_left(BL_Turn_Sensor);

  if (turn_fr_sensor || turn_bl_sensor){
    if (front_object_detected || front_detection) {
      state = state_backup;
    } else {
      state = state_forward;
    }
    
  } else {
    int random_turn_time = random(10, 50);
    for (int i = 0; i < 5; i++){
      right(BL_Wheel_Direction, BR_Wheel_Direction, FL_Wheel_Direction, FR_Wheel_Direction);
      enable_on(BL_Wheel_Enable, BR_Wheel_Enable, FL_Wheel_Enable, FR_Wheel_Enable);

      delay(random_turn_time);

      enable_off(BL_Wheel_Enable, BR_Wheel_Enable, FL_Wheel_Enable, FR_Wheel_Enable);
      if (turn_fr_sensor || turn_bl_sensor){
        if (front_object_detected || front_detection) {
          state = state_backup;
        } else {
          state = state_forward;
        }
        break;
      }
      state = state_forward;
    }
  }
}

void setup() {

  // Enable watchdog.
  WDT->WDT_MR = WDT_MR_WDD(0xFFF) |
                WDT_MR_WDFIEN |  //  Triggers an interrupt or WDT_MR_WDRSTEN to trigger a Reset
                WDT_MR_WDV(256 * 5); // Watchdog triggers a reset or an interrupt after 5 seconds if underflow
  // 2 seconds equal 84000000 * 5 = 420000000 clock cycles
  /* Slow clock is running at 32.768 kHz
    watchdog frequency is therefore 32768 / 128 = 256 Hz
    WDV holds the period in 256th of seconds  */
  NVIC_EnableIRQ(WDT_IRQn);

  // Front Line Follower Sensor Pin Out
  pinMode(FLFS_R_pin, INPUT);
  pinMode(FLFS_M_pin, INPUT);
  pinMode(FLFS_L_pin, INPUT);

  // Back Line Follower Sensor Pin Out
  pinMode(BLFS_R_pin, INPUT);
  pinMode(BLFS_M_pin, INPUT);
  pinMode(BLFS_L_pin, INPUT);

  // Stepper Motors Pin Out
  //Back Left
  pinMode(BL_Wheel_Enable, OUTPUT);
  pinMode(BL_Wheel_Direction, OUTPUT);
  //Back Right
  pinMode(BR_Wheel_Enable, OUTPUT);
  pinMode(BR_Wheel_Direction, OUTPUT);
  //Front Left
  pinMode(FL_Wheel_Enable, OUTPUT);
  pinMode(FL_Wheel_Direction, OUTPUT);
  //Front Right
  pinMode(FR_Wheel_Enable, OUTPUT);
  pinMode(FR_Wheel_Direction, OUTPUT);

  // Front Right IR Sensor
  pinMode(FR_Turn_Sensor, INPUT);
  // Back Left IR Sensor
  pinMode(BL_Turn_Sensor, INPUT);

  // Back GP2 Distance Sensor
  pinMode(GP2_Sensor, INPUT);

  // Ultrasonic Sensor
  pinMode(ultrasonic_trig, OUTPUT);
  pinMode(ultrasonic_echo, INPUT);

  Serial.begin(115200);

}

void loop()
{

  //Restart watchdog
  WDT->WDT_CR = WDT_CR_KEY(WDT_KEY)
                | WDT_CR_WDRSTT;

  switch (state) {
    case state_forward:
      state_forward_handler();
      break;
    case state_turn_right:
      state_turn_right_handler();
      break;
    case state_backup:
      state_backup_handler();
      break;
    case state_inch_forward:
      state_inch_forward_handler();
      break;
    case state_random_turn_right:
      state_random_turn_right_handler();
      break;
  }
}