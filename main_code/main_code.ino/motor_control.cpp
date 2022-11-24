#include "motor_control.h"
#include "Arduino.h"

void forward(int BL_Direction, int BR_Direction, int FL_Direction, int FR_Direction){
  digitalWrite(BL_Direction, LOW);
  digitalWrite(FL_Direction, LOW);
  digitalWrite(BR_Direction, HIGH);
  digitalWrite(FR_Direction, HIGH);
}

void backward(int BL_Direction, int BR_Direction, int FL_Direction, int FR_Direction){
  digitalWrite(BL_Direction, HIGH);
  digitalWrite(FL_Direction, HIGH);
  digitalWrite(BR_Direction, LOW);
  digitalWrite(FR_Direction, LOW);
}

void left(int BL_Direction, int BR_Direction, int FL_Direction, int FR_Direction){
  digitalWrite(BL_Direction, HIGH);
  digitalWrite(FL_Direction, HIGH);
  digitalWrite(BR_Direction, HIGH);
  digitalWrite(FR_Direction, HIGH);
}

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
  // 153 = 60% of 255: The amount of duty required to produce a speed of 30 cm/s at 1 khz frequency.
  analogWrite(BL_Enable, 153); 
  analogWrite(FL_Enable, 153);
  analogWrite(BR_Enable, 153);
  analogWrite(FR_Enable, 153);
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
