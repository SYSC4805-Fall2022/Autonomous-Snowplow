#define RIGHT false
#define LEFT true 

// Line Follower Sensor Pin Out
#define LFS_R_pin 23
#define LFS_M_pin 25
#define LSF_L_pin 27

////Stepper Motors Pin Out
//Back Left
#define BL_Wheel_Enable 47
#define BL_Wheel_Direction 2
//Back Right
#define BR_Wheel_Enable 51
#define BR_Wheel_Direction 4
//Front Left
#define FL_Wheel_Enable 49
#define FL_Wheel_Direction 3
//Front Right
#define FR_Wheel_Enable 53
#define FR_Wheel_Direction 5

void setup() {
  pinMode(LFS_R_pin, INPUT);   //LFS_R  OUT2
  pinMode(LFS_M_pin, INPUT);   //LFS_M  OUT3
  pinMode(LSF_L_pin, INPUT);   //LFS_L  OUT1

  // BL Wheel
  pinMode(BL_Wheel_Enable, OUTPUT); // Enable
  pinMode(BL_Wheel_Direction, OUTPUT); // Direction

  // BR Wheel
  pinMode(BR_Wheel_Enable, OUTPUT); // Enable
  pinMode(BR_Wheel_Direction, OUTPUT); // Direction

  // FL Wheel
  pinMode(FL_Wheel_Enable, OUTPUT); // Enable
  pinMode(FL_Wheel_Direction, OUTPUT); // Direction

  // FR Wheel
  pinMode(FR_Wheel_Enable, OUTPUT); // Enable
  pinMode(FR_Wheel_Direction, OUTPUT); // Direction
}

void loop() {
  // put your main code here, to run repeatedly:

}

void moveStraight(){
  // Directions all Forward
  digitalWrite(BL_Wheel_Direction, LOW);
  digitalWrite(FL_Wheel_Direction, LOW);
  digitalWrite(BR_Wheel_Direction, HIGH);
  digitalWrite(FR_Wheel_Direction, HIGH);

  //Enable 
  digitalWrite(BL_Wheel_Enable, HIGH);
  digitalWrite(FL_Wheel_Enable, HIGH);
  digitalWrite(BR_Wheel_Enable, HIGH);
  digitalWrite(FR_Wheel_Enable, HIGH);
}

void move_distance(){


}

void stop(){
  //Disable 
  digitalWrite(BL_Wheel_Enable, LOW);
  digitalWrite(FL_Wheel_Enable, LOW);
  digitalWrite(BR_Wheel_Enable, LOW);
  digitalWrite(FR_Wheel_Enable, LOW);

}

void turn27(bool direction){
  //Move Straight code

}

void turn45(){
  //Move Straight code

}

void turn90(){
  //Move Straight code

}

void turn180(){
  //Move Straight code

}