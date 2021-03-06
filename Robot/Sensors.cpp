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
  auto echoTime = sonar.ping_median(3);
  if (echoTime == 0)
  {
    return 160.f;
  }
  else
  {
      return (static_cast<float>(echoTime)/2.f) / 2.91f;
  }
}

UltraSonic::UltraSonic(unsigned trigPin, unsigned echoPin) :
sonar(trigPin, echoPin, 15.f)
{
}

float IRDist::getDist()
{
  // Convert to volts
  float volts = readValue() * 0.0048828125f;

  // Constrain
  volts = constrain(volts, vMin, vMax);

  // Convert to distance and return
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
