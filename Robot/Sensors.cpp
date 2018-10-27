#include "Sensors.h"
// Standard Includes

// Custom Includes
#include "Arduino.h"

UltraSonic::UltraSonic(unsigned pinInd) :
Sensor(pinInd, true)
{
}

IR::IR(unsigned pinInd) : 
Sensor(pinInd, false)
{
}


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
