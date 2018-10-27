// Standard Includes

// Custom Includes
#include "Types.h"
#include "Sensors.h"
#include "Actuators.h"

// Pin Assignements

Sensor test(0, true);
Actuator LED(9, true);
Servo s;

void setup() {
  
  Serial.begin(9600);              //  setup serial
  s.attach(2);
}

void loop() {
  
  int val = test.readValue();
  Serial.println(val);
  LED.set(50);
  delay(2000);
  LED.set(255);
  delay(2000);
  LED.set(0);
  delay(2000);
  
}
