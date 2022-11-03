int analogPin = A0;

void setup() {
  // put your setup code here, to run once:
  pinMode(analogPin, INPUT);   //A0
  // analogReference(EXTERNAL);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  float volt = ((float)analogRead(analogPin)/1023.0)*3.3;

  Serial.print("Section L1, Group 9, Voltage: ");
  Serial.print(volt);
  Serial.println("");
  
  delay(500);
}
