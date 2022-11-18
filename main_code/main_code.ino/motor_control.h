#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H
#include <Arduino.h>

void forward(int BL_Direction, int BR_Direction, int FL_Direction, int FR_Direction);
void backward(int BL_Direction, int BR_Direction, int FL_Direction, int FR_Direction);
void left(int BL_Direction, int BR_Direction, int FL_Direction, int FR_Direction);
void right(int BL_Direction, int BR_Direction, int FL_Direction, int FR_Direction);
void enable_on(int BL_Enable, int BR_Enable, int FL_Enable, int FR_Enable);
void enable_off(int BL_Enable, int BR_Enable, int FL_Enable, int FR_Enable);
void enable_left_turn(int BL_Enable, int BR_Enable, int FL_Enable, int FR_Enable);
void enable_right_turn(int BL_Enable, int BR_Enable, int FL_Enable, int FR_Enable);

#endif
