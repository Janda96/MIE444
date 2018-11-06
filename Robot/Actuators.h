#pragma once

// Standard Includes
#include <Servo.h>

// Custom Includes
#include "Types.h"

// Servo Positions
static const unsigned NeutralPos = 90;
static const unsigned ReleasePos = 180;
static const unsigned PickupPos = 0;

class Actuator
{
public:

  void set(int val);

  Actuator(unsigned pinInd, bool isPWM);

private: /* DATA */

  // Pin info
  unsigned pinInd;
  bool isPWM;
  
};
