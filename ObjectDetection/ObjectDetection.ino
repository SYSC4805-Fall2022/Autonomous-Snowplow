bool toggle = HIGH;
int detectCounter = 0;
bool ObjectDetected = LOW;

void setup() {
  // put your setup code here, to run once:
  // Object Detection Sensor
  pinMode(30, INPUT);

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
  changeDirection('f');

  Serial.begin(9600);
}

void changeDirection(char direction) {
  if (direction == 'b') {
    digitalWrite(2, HIGH);
    digitalWrite(3, HIGH);
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
  } else if (direction == 'f') {
    digitalWrite(2, LOW);
    digitalWrite(3, LOW);
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);
  } else if (direction == 'l') {
    digitalWrite(2, HIGH);
    digitalWrite(3, HIGH);
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);
  } else if (direction == 'r') {
    digitalWrite(2, LOW);
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  bool tempDetect = digitalRead(30);

  if (!tempDetect) {
  detectCounter += 1;
    if (detectCounter == 2) {
      ObjectDetected = HIGH;
      detectCounter = 0;
    }
  } else {
    detectCounter = 0;
  }

  if (!ObjectDetected) {
    changeDirection('f');
    digitalWrite(47, HIGH);
    digitalWrite(49, HIGH);
    digitalWrite(51, HIGH);
    digitalWrite(53, HIGH);
  } else {
    changeDirection('b');
    ObjectDetected = LOW;
    
    digitalWrite(47, HIGH);
    digitalWrite(49, HIGH);
    digitalWrite(51, HIGH);
    digitalWrite(53, HIGH);

    delay(1000);

    digitalWrite(47, LOW);
    digitalWrite(49, LOW);
    digitalWrite(51, LOW);
    digitalWrite(53, LOW);

    delay(500);
    
    changeDirection('f');

    long rngTime = random(100, 4000);
    long rngDirection = random(2);

    if (rngDirection == 0){
      changeDirection('r');
    }else {
      changeDirection('l');
    }

    digitalWrite(47, HIGH);
    digitalWrite(49, HIGH);
    digitalWrite(51, HIGH);
    digitalWrite(53, HIGH);

    delay(rngTime);

  }

}
