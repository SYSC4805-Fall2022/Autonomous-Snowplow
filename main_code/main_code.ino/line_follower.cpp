#include "line_follower.h"


int steer_direction(int FLFS_R_pin, int FLFS_M_pin, int FLFS_L_pin) { 
  
  bool LFS_R = digitalRead(FLFS_R_pin); // bool val for front line follower sensor (right)
  bool LFS_M = digitalRead(FLFS_M_pin); // bool val for front line follower sensor (middle)
  bool LFS_L = digitalRead(FLFS_L_pin); // bool val for front line follower sensor (left)

  // Straight = 1, left = 0, right = 2, corner = 3
  int dir = 1;

  if (LFS_L){  // Go straight
    dir = 1;
  } else if (!LFS_L){ // Go Left
    dir = 0;
  }

  if (LFS_R){  // Corner Detection
    dir = 3;
  }

  if (LFS_M){  // Go Right
    dir = 2;
  }

  return dir;
}

bool back_detection(int BLFS_R_pin, int BLFS_M_pin, int BLFS_L_pin){ // Check back sensor
  bool LFS_R = digitalRead(BLFS_R_pin); // bool val for back line follower sensor (right)
  bool LFS_M = digitalRead(BLFS_M_pin); // bool val for back line follower sensor (middle)
  bool LFS_L = digitalRead(BLFS_L_pin); // bool val for back line follower sensor (left)

  if (LFS_R || LFS_M || LFS_L){ 
    return true;
  }
  return false;
}

bool front_detection(int FLFS_R_pin, int FLFS_M_pin, int FLFS_L_pin){ // Check front sensor
  bool LFS_R = digitalRead(FLFS_R_pin); // bool val for front line follower sensor (right)
  bool LFS_M = digitalRead(FLFS_M_pin); // bool val for front line follower sensor (middle)
  bool LFS_L = digitalRead(FLFS_L_pin); // bool val for front line follower sensor (left)

  if (LFS_R || LFS_M || LFS_L){ 
    return true;
  }
  return false;
}
