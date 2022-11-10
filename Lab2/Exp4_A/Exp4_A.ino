int TRIG_PIN = 35;
int ECHO_PIN = 6;

void setup() {
  // put your setup code here, to run once:
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  float width = pulseIn(ECHO_PIN, HIGH, 50000);
  float distance = ((width/1000000)*340.0)/2.0;
  Serial.print("Section L1, Group 9, Distance: ");
  Serial.print(distance);
  Serial.println("");
  delay(60);
}
