/* Autonomous Snowplow Robot
* Class: SYSC 4805 Fall 2022
* Group: L1-G9, Team Name: Big Foot Feet
* Authors: Jacob Charpentier 101105745, Alina Ahmad 101111867, Julian Obando Velez 101124710
* Version 2.0
* Removed Phase 1 from algorithm Perimeter into Random to become Random Pathing algorithm, previous version can be seen on Github.
*/
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
#define BL_Wheel_Enable 11
#define BL_Wheel_Direction 3
//Back Right
#define BR_Wheel_Enable 9
#define BR_Wheel_Direction 5
//Front Left
#define FL_Wheel_Enable 10
#define FL_Wheel_Direction 4
//Front Right
#define FR_Wheel_Enable 8
#define FR_Wheel_Direction 6

// Ultrasonic Sensor 1
#define ultrasonic_trig1 2
#define ultrasonic_echo1 A7

// Ultrasonic Sensor 2
#define ultrasonic_trig2 13
#define ultrasonic_echo2 A6

// Ultrasonic control using the EZDist library
EZDist ezdist1 = EZDist(ultrasonic_trig1, ultrasonic_echo1);
EZDist ezdist2 = EZDist(ultrasonic_trig2, ultrasonic_echo2);

// WDT
#define WDT_KEY (0xA5)

// States
const int state_forward = 0;
const int state_backup = 1;
const int state_random_turn_right = 2;

// Maximum obstacle detection response threshold
const int main_object_detection_threshold = 15;

// Current state
volatile int state = state_forward;

/*
* Used to override default watchdogSetup so as to remove the watchdogDisable method call.
*/
void watchdogSetup(void) {
  /*** watchdogDisable (); ***/
}

/*
 * WatchDogTimer handler, enters the phase state_watchdog which does nothing after turning off the wheels. 
 */
void WDT_Handler(void)
{
  /* Clear status bit to acknowledge interrupt by dummy read. */
  WDT->WDT_SR; // Clear status register
  enable_off(BL_Wheel_Enable, BR_Wheel_Enable, FL_Wheel_Enable, FR_Wheel_Enable);
  printf("help! in WDT\n");
  int state_watchdog = -1;
  state = state_watchdog;
}

/* Snowplow will enter backup state if a line or front obstacle is detected, otherwise it keeps moving forward
 */
void state_forward_handler(){
  bool front_line_detected = front_detection(FLFS_R_pin, FLFS_M_pin, FLFS_L_pin);
  bool front_object_detected = object_detection_ultrasonic(ezdist1, main_object_detection_threshold);
  bool front_object_detected2 = object_detection_ultrasonic(ezdist2, main_object_detection_threshold);
  
  if(front_line_detected || front_object_detected || front_object_detected2) {
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
 * Backs up the snowplow for a determinate time
 */
void state_backup_handler(){
  // turn off motors before changing direction to preserve motor life
  enable_off(BL_Wheel_Enable, BR_Wheel_Enable, FL_Wheel_Enable, FR_Wheel_Enable);
  backward(BL_Wheel_Direction, BR_Wheel_Direction, FL_Wheel_Direction, FR_Wheel_Direction);
  enable_on(BL_Wheel_Enable, BR_Wheel_Enable, FL_Wheel_Enable, FR_Wheel_Enable);

  delay(600);    // Delay for 600 milliseconds (amount required to avoid corner collision reliably, as determined through testing)
  state = state_random_turn_right;
}

/*
 * Turns the snowplow right for a randomized time between 0.5 and 1 second
 */
void state_random_turn_right_handler(){
  int random_turn_time = random(500, 1000); // Determine random number

  right(BL_Wheel_Direction, BR_Wheel_Direction, FL_Wheel_Direction, FR_Wheel_Direction);
  enable_on(BL_Wheel_Enable, BR_Wheel_Enable, FL_Wheel_Enable, FR_Wheel_Enable);

  delay(random_turn_time);

  enable_off(BL_Wheel_Enable, BR_Wheel_Enable, FL_Wheel_Enable, FR_Wheel_Enable);
  state = state_forward;
}

/*
* Main Setup function, initializes watchdogtimer and all used pins, as well as begins serial communication at 115200 baud rate
*/
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

/*
* Main Loop, determines and runs each state accordingly. Restarts watchdog timer
*/
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
    case state_backup:
      state_backup_handler();
      break;
    case state_random_turn_right:
      state_random_turn_right_handler();
      break;
    default:
      break;
  }
}
