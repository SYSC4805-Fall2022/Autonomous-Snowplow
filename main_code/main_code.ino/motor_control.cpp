// Code will be used to control the wheels direction

#include "motor_control.h"
#include "Arduino.h"

//Robot moves forward when the back-right and front-right is set to HIGH
void forward(int BL_Direction, int BR_Direction, int FL_Direction, int FR_Direction){
  digitalWrite(BL_Direction, LOW);
  digitalWrite(FL_Direction, LOW);
  digitalWrite(BR_Direction, HIGH);
  digitalWrite(FR_Direction, HIGH);
}

//Robot moves backwards when back-left and front-left are HIGH
void backward(int BL_Direction, int BR_Direction, int FL_Direction, int FR_Direction){
  digitalWrite(BL_Direction, HIGH);
  digitalWrite(FL_Direction, HIGH);
  digitalWrite(BR_Direction, LOW);
  digitalWrite(FR_Direction, LOW);
}
//The robot turns left when all 4-wheels are HIGH
void left(int BL_Direction, int BR_Direction, int FL_Direction, int FR_Direction){
  digitalWrite(BL_Direction, HIGH);
  digitalWrite(FL_Direction, HIGH);
  digitalWrite(BR_Direction, HIGH);
  digitalWrite(FR_Direction, HIGH);
}

//default setting of the robot=> turn right 
void right(int BL_Direction, int BR_Direction, int FL_Direction, int FR_Direction){
  digitalWrite(BL_Direction, LOW);
  digitalWrite(FL_Direction, LOW);
  digitalWrite(BR_Direction, LOW);
  digitalWrite(FR_Direction, LOW);
}

void enable_on(int BL_Enable, int BR_Enable, int FL_Enable, int FR_Enable){
  // digitalWrite(BL_Enable, HIGH);
  // digitalWrite(FL_Enable, HIGH);
  // digitalWrite(BR_Enable, HIGH);
  // digitalWrite(FR_Enable, HIGH);
  analogWrite(BL_Enable, 200);
  analogWrite(FL_Enable, 200);
  analogWrite(BR_Enable, 200);
  analogWrite(FR_Enable, 200);
}

void enable_off(int BL_Enable, int BR_Enable, int FL_Enable, int FR_Enable){
  digitalWrite(BL_Enable, LOW);
  digitalWrite(FL_Enable, LOW);
  digitalWrite(BR_Enable, LOW);
  digitalWrite(FR_Enable, LOW);
}

void enable_left_turn(int BL_Enable, int BR_Enable, int FL_Enable, int FR_Enable){
  digitalWrite(BL_Enable, LOW);
  digitalWrite(FL_Enable, LOW);
  digitalWrite(BR_Enable, HIGH);
  digitalWrite(FR_Enable, HIGH);
}

void enable_right_turn(int BL_Enable, int BR_Enable, int FL_Enable, int FR_Enable){
  digitalWrite(BL_Enable, HIGH);
  digitalWrite(FL_Enable, HIGH);
  digitalWrite(BR_Enable, LOW);
  digitalWrite(FR_Enable, LOW);
}
