// Code will be used to control the wheels direction
#include "motor_control.h"
#include "Arduino.h"

/*
 * Sets the directions pins to move forward
 */
void forward(int BL_Direction, int BR_Direction, int FL_Direction, int FR_Direction){
  //Robot moves forward when the back-right and front-right is set to HIGH
  digitalWrite(BL_Direction, LOW);
  digitalWrite(FL_Direction, LOW);
  digitalWrite(BR_Direction, HIGH);
  digitalWrite(FR_Direction, HIGH);
}

/*
 * Sets the directions pins to move backwards
 */
void backward(int BL_Direction, int BR_Direction, int FL_Direction, int FR_Direction){
  //Robot moves backwards when back-left and front-left are HIGH
  digitalWrite(BL_Direction, HIGH);
  digitalWrite(FL_Direction, HIGH);
  digitalWrite(BR_Direction, LOW);
  digitalWrite(FR_Direction, LOW);
}

/*
 * Sets the directions pins to move right
 */
void right(int BL_Direction, int BR_Direction, int FL_Direction, int FR_Direction){
  //default setting of the robot=> turn right 
  digitalWrite(BL_Direction, LOW);
  digitalWrite(FL_Direction, LOW);
  digitalWrite(BR_Direction, LOW);
  digitalWrite(FR_Direction, LOW);
}

/*
 * Sets the enable pins to power on the motors (Tested and at 100% duty cycle speed was significantly less than 30 cm/s)
 */
void enable_on(int BL_Enable, int BR_Enable, int FL_Enable, int FR_Enable){
  digitalWrite(BL_Enable, HIGH);
  digitalWrite(FL_Enable, HIGH);
  digitalWrite(BR_Enable, HIGH);
  digitalWrite(FR_Enable, HIGH);
}

/*
 * Sets the enable pins to power off the motors
 */
void enable_off(int BL_Enable, int BR_Enable, int FL_Enable, int FR_Enable){
  digitalWrite(BL_Enable, LOW);
  digitalWrite(FL_Enable, LOW);
  digitalWrite(BR_Enable, LOW);
  digitalWrite(FR_Enable, LOW);
}
