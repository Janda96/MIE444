#pragma once
// Standard Includes
#include <SparkFun_TB6612.h>

// Custom Includes
#include "Types.h"
#include "IO.h"

class DriveTrain
{
public:

  void Drive(char vel, Direction d);

  void Drive(char vel, int dist, Direction d);

  void Stop();

  void Turn(float angle);

  DriveTrain(Motor L, Motor R, float rad);

private:

  // Control loop to make sure driving straight
  void DriveStraight();

private: /* DATA */
  
  Motor L;
  Motor R;
  float rad;
};
