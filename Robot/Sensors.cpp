#include "Sensors.h"

// Standard Includes

// Custom Includes
#include "Arduino.h"

// IR Dist tunning
#define vAt5cm   2.27f
#define vAt30cm  0.38f
#define vMin 0.2f
#define vMax 2.7f

static float IRDTunning = 250.f / (vAt5cm - vAt30cm);

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
//  float sum = 0.f;
//  for (auto i = 0; i < 3; ++i)
//  {
//    // Convert to volts
//    float volts = readValue() * 0.0048828125f;
//
//    // Convert to distance
//    // return 13.f * pow(volts, -1);
//    // sum += volts;
//    sum += IRDTunning / volts;
//  }
//  return sum / 3.f;
  
  // Convert to volts
  float volts = readValue() * 0.0048828125f;
  volts = constrain(volts, vMin, vMax);
  return IRDTunning / volts;
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
