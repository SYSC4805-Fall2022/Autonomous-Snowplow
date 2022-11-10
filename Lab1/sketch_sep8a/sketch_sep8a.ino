void setup() {
  // put your setup code here, to run once:
  pinMode(13, OUTPUT);
  pinMode(6, INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

if (digitalRead(6) == HIGH){
  digitalWrite(13,LOW);
  Serial.println("Group 9 >> No obstacles (Sensor reading is 1)");
} else {
  digitalWrite(13,HIGH);
  Serial.println("Group 9 >> Obstacle detected (Sensor reading is 0)");
}
// digitalWrite(13, !digitalRead(6));
delay(500);
}