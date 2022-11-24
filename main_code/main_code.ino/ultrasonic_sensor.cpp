#include "ultrasonic_sensor.h"

int get_distance_cm(EZDist object){
  int dist = object.cm();
  if (dist > 3){
    return dist;
  }
  return 100;
}

int get_average_distance_cm(EZDist object, int num_iterations){
  int total = 0;
  for(int i = 0; i < num_iterations; i++){
    total += object.cm();
  }
  return total / num_iterations;
}

bool object_detection_ultrasonic(EZDist object, int threshold_distance_cm){
  int dist = get_distance_cm(object);
  if (dist < threshold_distance_cm){
    Serial.println(dist);
    return true;
  }
  return false;
}