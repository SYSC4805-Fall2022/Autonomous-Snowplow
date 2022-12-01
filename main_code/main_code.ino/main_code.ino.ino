#include "line_follower.h"
#include "ultrasonic_sensor.h"
#include "motor_control.h"
#include <Arduino.h>

// Front Line Follower Sensor Pin Out
#define FLFS_R_pin 23
#define FLFS_M_pin 25
#define FLFS_L_pin 27

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

// Ultrasonic Sensor 1
#define ultrasonic_trig1 2
#define ultrasonic_echo1 A7
// EZDist EZDist(ultrasonic_trig, ultrasonic_echo);

// Ultrasonic Sensor 2
#define ultrasonic_trig2 13
#define ultrasonic_echo2 A6

EZDist ezdist1 = EZDist(ultrasonic_trig1, ultrasonic_echo1);
EZDist ezdist2 = EZDist(ultrasonic_trig2, ultrasonic_echo2);

// WDT
#define WDT_KEY (0xA5)
volatile int phase = 1;

// States
const int state_forward = 0;
const int state_turn_right = 1;
const int state_backup = 2;
const int state_inch_forward = 3;
const int state_random_turn_right = 4;

const int main_object_detection_threshold = 15;
const int short_object_detection_threshold = 10;
const int max_stuck_counter = 5;

volatile int state = state_forward;
volatile bool line_following = false;
volatile int right_turns = 0;
volatile int stuck_counter = 0;

void watchdogSetup(void) {
  /*** watchdogDisable (); ***/
}

void WDT_Handler(void)
{
  if (phase == 1){
    phase = 2;
    line_following = false;
    //Restart watchdog
    WDT->WDT_CR = WDT_CR_KEY(WDT_KEY)
                  | WDT_CR_WDRSTT;
  } else {
    /* Clear status bit to acknowledge interrupt by dummy read. */
    WDT->WDT_SR; // Clear status register
    enable_off(BL_Wheel_Enable, BR_Wheel_Enable, FL_Wheel_Enable, FR_Wheel_Enable);
    printf("help! in WDT\n");
  }
}

void state_forward_handler(){
  // TODO
  bool front_line_detected = front_detection(FLFS_R_pin, FLFS_M_pin, FLFS_L_pin);
  // bool front_line_detected = false;
  // bool front_object_detected = object_detection_ultrasonic(ezdist1, main_object_detection_threshold) || object_detection_ultrasonic(ezdist2, main_object_detection_threshold) || object_detection_gp2(GP2_Sensor_F, main_object_detection_threshold);
  // bool front_object_detected = object_detection_ultrasonic(ezdist1, main_object_detection_threshold) || object_detection_ultrasonic(ezdist2, main_object_detection_threshold);
  bool front_object_detected = object_detection_ultrasonic(ezdist1, main_object_detection_threshold);
  bool front_object_detected2 = object_detection_ultrasonic(ezdist2, main_object_detection_threshold);
  // bool front_object_detected = false;
  stuck_counter = 0;
  Serial.println("FRONT OBJECT: ");
  Serial.println(front_object_detected || front_object_detected2);

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
  }else if(front_line_detected || front_object_detected || front_object_detected2) {
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
      
  for (int i = 0; i < 3; i++){
    // TODO
    // bool back_line_detected = back_detection(BLFS_R_pin, BLFS_M_pin, BLFS_L_pin);
    bool back_line_detected = false;
    bool back_object_detected = false;
    // bool back_object_detected = object_detection_gp2(GP2_Sensor_B, short_object_detection_threshold);
    Serial.println(back_line_detected);
    Serial.println(back_object_detected);
    Serial.println("");
    if (back_line_detected || back_object_detected){
      enable_off(BL_Wheel_Enable, BR_Wheel_Enable, FL_Wheel_Enable, FR_Wheel_Enable);
      // state = state_inch_forward;
      break;
    } else {
      enable_on(BL_Wheel_Enable, BR_Wheel_Enable, FL_Wheel_Enable, FR_Wheel_Enable);
      delay(200);
    }
  }
  state = state_random_turn_right;
  // TODO
  // if (state == state_backup){
  //   state = state_random_turn_right;
  // }
}

void state_inch_forward_handler(){
  // TODO
  // bool front_line_detected = front_detection(FLFS_R_pin, FLFS_M_pin, FLFS_L_pin);
  // bool front_object_detected = object_detection_ultrasonic(ezdist1, main_object_detection_threshold) || object_detection_ultrasonic(ezdist2, main_object_detection_threshold) || object_detection_gp2(GP2_Sensor_F, main_object_detection_threshold);

  // if(!front_line_detected || !front_object_detected){
  //   forward(BL_Wheel_Direction, BR_Wheel_Direction, FL_Wheel_Direction, FR_Wheel_Direction);
  //   enable_on(BL_Wheel_Enable, BR_Wheel_Enable, FL_Wheel_Enable, FR_Wheel_Enable);
  //   delay(100);
  //   enable_off(BL_Wheel_Enable, BR_Wheel_Enable, FL_Wheel_Enable, FR_Wheel_Enable);
  //   state = state_random_turn_right;
  // } else if(stuck_counter == max_stuck_counter){
  //   state = state_random_turn_right; // We are stuck, we should try to get out
  // } else {
  //   state = state_backup;
  //   stuck_counter += 1;
  // }
  forward(BL_Wheel_Direction, BR_Wheel_Direction, FL_Wheel_Direction, FR_Wheel_Direction);
  enable_on(BL_Wheel_Enable, BR_Wheel_Enable, FL_Wheel_Enable, FR_Wheel_Enable);
  delay(100);
  enable_off(BL_Wheel_Enable, BR_Wheel_Enable, FL_Wheel_Enable, FR_Wheel_Enable);
  state = state_random_turn_right;
}

void state_random_turn_right_handler(){
  // TODO
  // bool front_line_detected = front_detection(FLFS_R_pin, FLFS_M_pin, FLFS_L_pin);
  // bool front_object_detected = object_detection_ultrasonic(ezdist1, main_object_detection_threshold) || object_detection_ultrasonic(ezdist2, main_object_detection_threshold) || object_detection_gp2(GP2_Sensor_F, main_object_detection_threshold);
  // bool turn_fr_sensor = turn_check_front_right(FR_Turn_Sensor);
  // bool turn_bl_sensor = turn_check_back_left(BL_Turn_Sensor);
  bool front_line_detected = false;
  bool front_object_detected = false;
  bool turn_fr_sensor = false;
  bool turn_bl_sensor = false;

  int random_turn_time = random(500, 1000);
  for (int i = 0; i < 5; i++){
    Serial.println("Right Turn");
    right(BL_Wheel_Direction, BR_Wheel_Direction, FL_Wheel_Direction, FR_Wheel_Direction);
    enable_on(BL_Wheel_Enable, BR_Wheel_Enable, FL_Wheel_Enable, FR_Wheel_Enable);

    delay(random_turn_time);

    enable_off(BL_Wheel_Enable, BR_Wheel_Enable, FL_Wheel_Enable, FR_Wheel_Enable);

    // TODO
    // front_line_detected = front_detection(FLFS_R_pin, FLFS_M_pin, FLFS_L_pin);
    // front_object_detected = object_detection_ultrasonic(ezdist1, main_object_detection_threshold) || object_detection_ultrasonic(ezdist2, main_object_detection_threshold) || object_detection_gp2(GP2_Sensor_F, main_object_detection_threshold);
    // turn_fr_sensor = turn_check_front_right(FR_Turn_Sensor);
    // turn_bl_sensor = turn_check_back_left(BL_Turn_Sensor);

    // if (turn_fr_sensor || turn_bl_sensor){
    //   // if (front_object_detected || front_detection) {
    //   //   state = state_backup;
    //   // } else {
    //   //   state = state_forward;
    //   // }
    //   Serial.println("SIDE");
    //   state = state_backup;
    //   break;
    // }
    state = state_forward;
    break;
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

  // Ultrasonic Sensors
  pinMode(ultrasonic_trig1, OUTPUT);
  pinMode(ultrasonic_echo1, INPUT);
  pinMode(ultrasonic_trig2, OUTPUT);
  pinMode(ultrasonic_echo2, INPUT);

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