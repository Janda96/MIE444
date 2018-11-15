#pragma once
// Standard Includes

// Custom Includes
#include "Types.h"
#include "Sensors.h"

class Loc
{
public:

  void GetPose(point& p, Orientationfloat var);

  void TakeMeasurement();

  Loc(IRArray& IRA);
  
private:

  IRArray IRA;
};
