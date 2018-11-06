#pragma once

// Standard Includes

// Custom Includes
#include "Types.h"

class Output
{
public:

  void set(int val);

  Output(unsigned pinInd, bool isPWM);

private: /* DATA */

  // Pin info
  unsigned pinInd;
  bool isPWM;
  
};

class Input
{
public:

  int readValue();

  unsigned long timePulse(int value);

  Input(unsigned pinInd, bool isAnalog);

private: /* DATA */

  // Pin info
  unsigned pinInd;
  bool isAnalog;
};
