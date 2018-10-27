#pragma once
// Standard Includes

// Custom Includes
#include "Sensors.h"

class Locator
{
public:

  ErrorCode getPosition(float& x, float& y);
  
  Locator(SensorAndPos<IR> groundDetector[4], SensorAndPos<UltraSonic> wallDetector[4]);
  
private: /* DATA */

  // IR Colour Sensors
  SensorAndPos<IR>* groundDetector;

  // UltraSonic Wall Detectors
  SensorAndPos<UltraSonic>* wallDetector;
  
};
