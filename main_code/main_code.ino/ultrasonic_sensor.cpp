# The Ultrasonic sensor is used to measures the distance between the the robot and a potential obstacle
# The EZDist object zip file is used to get the distance in centimeters and uses this distance to find the average distance


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

//Returns True if object distance is less than the threhold distance
//Detects an obstacle while the robot is moving within the taped-area

bool object_detection_ultrasonic(EZDist object, int threshold_distance_cm){
  if (get_distance_cm(object) < threshold_distance_cm){
    return true;
  }
  return false;
}
