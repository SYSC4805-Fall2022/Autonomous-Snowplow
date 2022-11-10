//Line Follower Pin Out
// #Define LFS_R 23
// #Define LFS_M 25
// #Define LSF_L 27


void setup() {
  // put your setup code here, to run once:
  pinMode(23, INPUT);   //LFS_R  OUT2
  pinMode(25, INPUT);   //LFS_M  OUT3
  pinMode(27, INPUT);   //LFS_L  OUT1

  // BL Wheel
  pinMode(47, OUTPUT); // Enable
  pinMode(2, OUTPUT); // Direction

  // BR Wheel
  pinMode(51, OUTPUT); // Enable
  pinMode(4, OUTPUT); // Direction

  // FL Wheel
  pinMode(49, OUTPUT); // Enable
  pinMode(3, OUTPUT); // Direction

  // FR Wheel
  pinMode(53, OUTPUT); // Enable
  pinMode(5, OUTPUT); // Direction

  // Directions all Forward
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH);

  // Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

bool LFS_R = digitalRead(23);
bool LFS_M = digitalRead(25);
bool LFS_L = digitalRead(27);

// Serial.print("L1, Group9 >> ");

if (LFS_M){ // Go Straight
  // Serial.print("Middle: B - ");
  digitalWrite(47, HIGH);
  digitalWrite(49, HIGH);
  digitalWrite(51, HIGH);
  digitalWrite(53, HIGH);
}

if (LFS_L){
  // Serial.print("Left: B - ");
  digitalWrite(47, LOW);
  digitalWrite(49, LOW);
  digitalWrite(51, HIGH);
  digitalWrite(53, HIGH);
}

if (LFS_R){
  // Serial.print("Right: B - ");
  digitalWrite(47, HIGH);
  digitalWrite(49, HIGH);
  digitalWrite(51, LOW);
  digitalWrite(53, LOW);
}

// Serial.println("");

delay(100);
}
