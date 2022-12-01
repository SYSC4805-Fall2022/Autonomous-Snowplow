//The Ultrasonic sensor is used to measures the distance between the the robot and a potential obstacle
//The EZDist object zip file is used to get the distance in centimeters and uses this distance to find the average distance


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
 * Returns if object detected within a threshold distance
 */
bool object_detection_ultrasonic(EZDist object, int threshold_distance_cm){
  //Returns True if object distance is less than the threhold distance
  //Detects an obstacle while the robot is moving within the taped-area
  int dist = get_distance_cm(object);
  if (dist < threshold_distance_cm){
    Serial.println(dist);
    return true;
  }
  return false;
}
