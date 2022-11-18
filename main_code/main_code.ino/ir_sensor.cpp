#include "ir_sensor.h"

bool turn_check_front_right(int FR_Pin){
  int counter = 0;
  for (int i=0; i < 3; i++){
    bool temp_detect = digitalRead(FR_Pin);
    if (temp_detect){
      counter += 1;
    }
    if (counter == 3){
      return true;
    }
  }
  return false;
}

bool turn_check_back_left(int BL_Pin){
  int counter = 0;
  for (int i=0; i < 3; i++){
    bool temp_detect = digitalRead(BL_Pin);
    if (temp_detect){
      counter += 1;
    }
    if (counter == 3){
      return true;
    }
  }
  return false;
}