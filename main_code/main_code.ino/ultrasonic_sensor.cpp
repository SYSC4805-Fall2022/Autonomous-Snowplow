#include "ultrasonic_sensor.h"

int get_distance_cm(EZDist object){
  return object.cm();
}

int get_average_distance_cm(EZDist object, int num_iterations){
  int total = 0;
  for(int i = 0; i < num_iterations; i++){
    total += object.cm();
  }
  return total / num_iterations;
}