#include "Sensor.h"
// Standard Includes

// Custom Includes
#include "Arduino.h"

Sensor::Sensor(unsigned pinInd, bool isAnalog) :
pinInd(pinInd),
isAnalog(isAnalog)
{
  pinMode(pinInd, INPUT);
}

int Sensor::readValue()
{
  return isAnalog ? analogRead(pinInd) : digitalRead(pinInd);
}
