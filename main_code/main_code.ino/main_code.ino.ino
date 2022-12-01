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

/*
 * Watch Dog handler goes into phase 1 the first time the timer is triggered 
 * The second time it mandates the robot plow to stop. 
 */
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

/*
 * In Phase 1 (Line Following), the robot uses the line detection to follow the line
 * In Phase 2, the car moves forward while checking for obstacles.
 */
void state_forward_handler(){
  bool front_line_detected = front_detection(FLFS_R_pin, FLFS_M_pin, FLFS_L_pin);
  bool front_object_detected = object_detection_ultrasonic(ezdist1, main_object_detection_threshold);
  bool front_object_detected2 = object_detection_ultrasonic(ezdist2, main_object_detection_threshold);
  stuck_counter = 0;
  //Serial.println("FRONT OBJECT: ");                                    Debugging...
  //Serial.println(front_object_detected || front_object_detected2);     Debugging...

  if(line_following){
    //Following the line perimeter
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
    //Object detected, go into back up and random turn routine
    enable_off(BL_Wheel_Enable, BR_Wheel_Enable, FL_Wheel_Enable, FR_Wheel_Enable);
    state = state_backup;
  }else{
    //Move forward
    forward(BL_Wheel_Direction, BR_Wheel_Direction, FL_Wheel_Direction, FR_Wheel_Direction);
    enable_on(BL_Wheel_Enable, BR_Wheel_Enable, FL_Wheel_Enable, FR_Wheel_Enable);   
  }
}

/*
 *  Turns the car 90degrees at the boundary edge until all of the perimeter reached
 */
void state_turn_right_handler(){
  right(BL_Wheel_Direction, BR_Wheel_Direction, FL_Wheel_Direction, FR_Wheel_Direction);
  enable_on(BL_Wheel_Enable, BR_Wheel_Enable, FL_Wheel_Enable, FR_Wheel_Enable);
  
  delay(500);

  enable_off(BL_Wheel_Enable, BR_Wheel_Enable, FL_Wheel_Enable, FR_Wheel_Enable);
  delay(100);
  forward(BL_Wheel_Direction, BR_Wheel_Direction, FL_Wheel_Direction, FR_Wheel_Direction);
  enable_on(BL_Wheel_Enable, BR_Wheel_Enable, FL_Wheel_Enable, FR_Wheel_Enable);
  
  //Counter for how many corners have been taken.
  right_turns += 1;
  if (right_turns == 5){
    line_following = false;
  }
  state = state_forward;
}

/*
 * Backs up the robot plow for a determinate time
 */
void state_backup_handler(){
  enable_off(BL_Wheel_Enable, BR_Wheel_Enable, FL_Wheel_Enable, FR_Wheel_Enable);
  backward(BL_Wheel_Direction, BR_Wheel_Direction, FL_Wheel_Direction, FR_Wheel_Direction);
  enable_on(BL_Wheel_Enable, BR_Wheel_Enable, FL_Wheel_Enable, FR_Wheel_Enable);
  delay(600);    //Equivalent to three cycles in a for loop
  state = state_random_turn_right;
}

/*
 * Forwards the robot plow for a determinate time
 */
void state_inch_forward_handler(){
  forward(BL_Wheel_Direction, BR_Wheel_Direction, FL_Wheel_Direction, FR_Wheel_Direction);
  enable_on(BL_Wheel_Enable, BR_Wheel_Enable, FL_Wheel_Enable, FR_Wheel_Enable);
  delay(100);
  enable_off(BL_Wheel_Enable, BR_Wheel_Enable, FL_Wheel_Enable, FR_Wheel_Enable);
  state = state_random_turn_right;
}

/*
 * Turns the robot plow for a randomized time
 */
void state_random_turn_right_handler(){
  int random_turn_time = random(500, 1000);
  //Serial.println("Right Turn");                 Debugging...
  right(BL_Wheel_Direction, BR_Wheel_Direction, FL_Wheel_Direction, FR_Wheel_Direction);
  enable_on(BL_Wheel_Enable, BR_Wheel_Enable, FL_Wheel_Enable, FR_Wheel_Enable);

  delay(random_turn_time);

  enable_off(BL_Wheel_Enable, BR_Wheel_Enable, FL_Wheel_Enable, FR_Wheel_Enable);
  state = state_forward;
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

  //Sends the state machine into the proper handler
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