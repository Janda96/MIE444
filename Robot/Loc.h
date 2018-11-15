#pragma once
// Standard Includes

// Custom Includes
#include "Types.h"
#include "Sensors.h"

class Loc
{
public:

  void GetLocation(point& p, float var);

  void TakeMeasurement();

  Loc(IRArray& IRA);
  
private:

  IRArray IRA;
};
