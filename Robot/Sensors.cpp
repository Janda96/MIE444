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

  float mm = (static_cast<float>(timeMicro)/2.f) / 2.91f;
  return mm;
}

UltraSonic::UltraSonic(unsigned trigPin, unsigned echoPin) :
echo(echoPin, false),
trig(trigPin, false)
{
}

float IRDist::getDist()
{
  // Convert to volts
  float volts = readValue() * 0.0048828125f;

  // Convert to distance
  return 13.f * pow(volts, -1);
}

IRDist::IRDist(unsigned pinInd) : 
Input(pinInd, true)
{
}

float IR::getBW()
{
  return readValue();
}

IR::IR(unsigned pinInd) : 
Input(pinInd, false)
{
}
