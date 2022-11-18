#ifndef LINE_FOLLOWER_H
#define LINE_FOLLOWER_H
#include <Arduino.h>

int steer_direction(int FLFS_R_pin, int FLFS_M_pin, int FLFS_L_pin);
bool back_detection(int BLFS_R_pin, int BLFS_M_pin, int BLFS_L_pin);
bool front_detection(int FLFS_R_pin, int FLFS_M_pin, int FLFS_L_pin);

#endif
