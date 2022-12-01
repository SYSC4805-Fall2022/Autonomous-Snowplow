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
 * Sets the directions pins to turn left
 */
void left(int BL_Direction, int BR_Direction, int FL_Direction, int FR_Direction){
  //The robot turns left when all 4-wheels are HIGH
  digitalWrite(BL_Direction, HIGH);
  digitalWrite(FL_Direction, HIGH);
  digitalWrite(BR_Direction, HIGH);
  digitalWrite(FR_Direction, HIGH);
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
 * Sets the enable pins to power on the motors
 */
void enable_on(int BL_Enable, int BR_Enable, int FL_Enable, int FR_Enable){
  digitalWrite(BL_Enable, HIGH);
  digitalWrite(FL_Enable, HIGH);
  digitalWrite(BR_Enable, HIGH);
  digitalWrite(FR_Enable, HIGH);
  // 153 = 60% of 255: The amount of duty required to produce a speed of 30 cm/s at 1 khz frequency.
  // analogWrite(BL_Enable, 153); 
  // analogWrite(FL_Enable, 153);
  // analogWrite(BR_Enable, 153);
  // analogWrite(FR_Enable, 153);
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

/*
 * Sets the enable pins to power the right motors only (to turn left)
 */
void enable_left_turn(int BL_Enable, int BR_Enable, int FL_Enable, int FR_Enable){
  digitalWrite(BL_Enable, LOW);
  digitalWrite(FL_Enable, LOW);
  digitalWrite(BR_Enable, HIGH);
  digitalWrite(FR_Enable, HIGH);
}

/*
 * Sets the enable pins to power the left motors only (to turn right)
 */
void enable_right_turn(int BL_Enable, int BR_Enable, int FL_Enable, int FR_Enable){
  digitalWrite(BL_Enable, HIGH);
  digitalWrite(FL_Enable, HIGH);
  digitalWrite(BR_Enable, LOW);
  digitalWrite(FR_Enable, LOW);
}
