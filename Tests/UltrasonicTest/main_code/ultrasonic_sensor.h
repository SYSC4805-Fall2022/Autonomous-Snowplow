#ifndef ULTRASONIC_SENSOR_H
#define ULTRASONIC_SENSOR_H
#include <Arduino.h>
#include <EZDist.h>

int get_distance_cm(EZDist object);
int get_average_distance_cm(EZDist object, int num_iterations);
bool object_detection_ultrasonic(EZDist object, int threshold_distance_cm);

#endif