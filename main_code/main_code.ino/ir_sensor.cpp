#include "ir_sensor.h"

bool turn_check_front_right(int FR_Pin){ //used to check the front right side before turning
  int counter = 0;
  for (int i=0; i < 3; i++){
    bool temp_detect = digitalRead(FR_Pin);//the Front right pin is read
    if (temp_detect){ //object detected
      counter += 1;
    }
    if (counter == 3){
      return true;
    }
  }
  return false;
}

bool turn_check_back_left(int BL_Pin){ //checking the back left sensor reading before turning
  int counter = 0;
  for (int i=0; i < 3; i++){ 
    bool temp_detect = digitalRead(BL_Pin); //Back left sensor pin is read 
    if (temp_detect){ //obstacle detected 
      counter += 1;
    }
    if (counter == 3){
      return true;
    }
  }
  return false;
}
