#include "distance_gp2.h"

double get_voltage(uint32_t pin){
  return ((double)analogRead(pin)/1023.0)*3.3;
}

int get_distance_gp2_cm(uint32_t pin){
  double volt = get_voltage(pin);
  int distance = (int) (3.56*pow(volt, -0.868));
  // Serial.println(distance);
  if (distance > 10){
    return distance;
  }
  return 100;
}

bool object_detection_gp2(uint32_t pin, int threshold_distance_cm){
  if (get_distance_gp2_cm(pin) < threshold_distance_cm){
    return true;
  }
  return false;
}