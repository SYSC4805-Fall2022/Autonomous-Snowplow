#ifndef ULTRASONIC_SENSOR_H
#define ULTRASONIC_SENSOR_H
#include <Arduino.h>
#include <EZDist.h>

int get_distance_cm(EZDist object);
bool object_detection_ultrasonic(EZDist object, int threshold_distance_cm);

#endif