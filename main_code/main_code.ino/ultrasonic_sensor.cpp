#include "ultrasonic_sensor.h"

/*
 * Returns the distance to the object detected in cm
 */
int get_distance_cm(EZDist object){
  int dist = object.cm();
  if (dist > 5){
    return dist;
  }
  return 100;
}

/*
 * Returns the distance to the object detected in cm with debouncing
 */
int get_average_distance_cm(EZDist object, int num_iterations){
  int total = 0;
  for(int i = 0; i < num_iterations; i++){
    total += object.cm();
  }
  return total / num_iterations;
}

/*
 * Returns if object detected within a threshold distance
 */
bool object_detection_ultrasonic(EZDist object, int threshold_distance_cm){
  int dist = get_distance_cm(object);
  if (dist < threshold_distance_cm){
    Serial.println(dist);
    return true;
  }
  return false;
}