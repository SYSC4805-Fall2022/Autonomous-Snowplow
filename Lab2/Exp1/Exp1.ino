void setup() {
  // put your setup code here, to run once:
  pinMode(2, INPUT);   //LFS_R  OUT2
  pinMode(3, INPUT);   //LFS_M  OUT3
  pinMode(7, INPUT);   //LFS_L  OUT1
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

bool LFS_R = digitalRead(2);
bool LFS_M = digitalRead(3);
bool LFS_L = digitalRead(7);

Serial.print("L1, Group9 >> ");

if (LFS_L){
  Serial.print("Left: B - ");
} else {
  Serial.print("Left: W - ");
}

if (LFS_M){
  Serial.print("Middle: B - ");
} else {
  Serial.print("Middle: W - ");
}

if (LFS_R){
  Serial.print("Right: B - ");
} else {
  Serial.print("Right: W - ");
}

Serial.println("");

delay(500);
}
