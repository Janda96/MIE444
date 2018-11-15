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

IR::IR(unsigned pinInd) : 
Input(pinInd, false)
{
}
