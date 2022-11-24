#include "ultrasonic_sensor.h"

// Ultrasonic Sensor 1
#define ultrasonic_trig1 2
#define ultrasonic_echo1 A7
// EZDist EZDist(ultrasonic_trig, ultrasonic_echo);

// Ultrasonic Sensor 2
#define ultrasonic_trig2 13
#define ultrasonic_echo2 A6
EZDist ezdist1 = EZDist(ultrasonic_trig1, ultrasonic_echo1);
EZDist ezdist2 = EZDist(ultrasonic_trig2, ultrasonic_echo2);

const int main_object_detection_threshold = 25;

bool front_object_detected1;
bool front_object_detected2;

void setup() {
  // Ultrasonic Sensor
  pinMode(ultrasonic_trig1, OUTPUT);
  pinMode(ultrasonic_echo1, INPUT);
  pinMode(ultrasonic_trig2, OUTPUT);
  pinMode(ultrasonic_echo2, INPUT);

  Serial.begin(115200);

}

void loop() {
  // put your main code here, to run repeatedly:
  front_object_detected1 = object_detection_ultrasonic(ezdist1, main_object_detection_threshold);
  front_object_detected2 = object_detection_ultrasonic(ezdist2, main_object_detection_threshold);
  if (front_object_detected1 || front_object_detected2){
    Serial.print("Object Detected\n");
  } else {
    Serial.print("Object Not Detected\n");
  }
  delay(500);

}
