#include "line_follower.h"

/*
 * Determines if front sensor detected a line
 */
bool front_detection(int FLFS_R_pin, int FLFS_M_pin, int FLFS_L_pin){ // Check front sensor
  bool LFS_R = digitalRead(FLFS_R_pin); // bool val for front line follower sensor (right)
  bool LFS_M = digitalRead(FLFS_M_pin); // bool val for front line follower sensor (middle)
  bool LFS_L = digitalRead(FLFS_L_pin); // bool val for front line follower sensor (left)

  if (LFS_R || LFS_M || LFS_L){ 
    return true;
  }
  return false;
}
