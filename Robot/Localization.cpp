#include "Localization.h"
// Standard Includes

// Custom Includes
#include "Arduino.h"

Locator::Locator(SensorAndPos<IR> groundDetector[4], SensorAndPos<UltraSonic> wallDetector[4]) : 
groundDetector(groundDetector),
wallDetector(wallDetector)
{
}

ErrorCode Locator::getPosition(float& x, float& y)
{
  x = 0;
  y = 0;
  return OK;
}
