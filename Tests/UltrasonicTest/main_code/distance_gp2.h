#ifndef DISTANCE_GP2_H
#define DISTANCE_GP2_H
#include <Arduino.h>

double get_voltage(uint32_t pin);
int get_distance_gp2_cm(uint32_t pin);
bool object_detection_gp2(uint32_t pin, int threshold_distance_cm);

#endif