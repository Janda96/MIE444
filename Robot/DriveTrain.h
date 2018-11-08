#pragma once
// Standard Includes
#include <SparkFun_TB6612.h>

// Custom Includes
#include "Types.h"
#include "IO.h"
#include "Sensors.h"

class DriveTrain
{
public:

  ErrorCode Drive();
  
  void Drive(char vel, Direction d);

  void Drive(char vel, int dist, Direction d);

  void Stop();

  void Turn(float angle);

  DriveTrain(Motor L, Motor R, UltraSonicArray US, float wheelbase);

private:

  ErrorCode FindFollower(UltraSonic* follower);

  ErrorCode ClearObstacle();

  bool isObsticalDetected();
  
  // Control loop to make sure driving straight
  void UpdateSpeed(float wallDist);

  void ResetSpeed();

private: /* DATA */

  Motor L;            // Left motor
  Motor R;            // Right motor
  UltraSonicArray US; // Ultrasonic array on robot              
  float wheelbase;    // Distance between wheels

  // Control parameters
  float targetDist = 50.f;  // Target distance to follow wall
  float k = 0.1f;           // Proportional gain
  char rSpeed;              // Right motor speed
  char lSpeed;              // Left motor speed

  ErrorCode m_err = OK;
};
