#include "Sensors.h"
// Standard Includes

// Custom Includes
#include "Arduino.h"

float UltraSonic::getDist()
{
  trig.set(LOW);
  delayMicroseconds(5);
  trig.set(HIGH);
  delayMicroseconds(10);
  trig.set(LOW);

  auto timeMicro = echo.timePulse(HIGH);

  float mm = (static_cast<float>(timeMicro)/2.f) / 2.91f / 25.4f;
  return mm;
}

UltraSonic::UltraSonic(unsigned trigPin, unsigned echoPin) :
echo(echoPin, false),
trig(trigPin, false)
{
  
}

IR::IR(unsigned pinInd) : 
Sensor(pinInd, false)
{
}

int Sensor::readValue()
{
  return isAnalog ? analogRead(pinInd) : digitalRead(pinInd);
}

unsigned long Sensor::timePulse(int value)
{
  return pulseIn(pinInd, value);
}

Sensor::Sensor(unsigned pinInd, bool isAnalog) :
pinInd(pinInd),
isAnalog(isAnalog)
{
  pinMode(pinInd, INPUT);
}
