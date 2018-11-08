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
  
  void Drive(int vel, Direction d);

  void Drive(int vel, int dist, Direction d);

  void Stop();

  void Turn(float angle);

  DriveTrain(Motor L, Motor R, UltraSonicArray US, float wheelbase);

private:

  ErrorCode FindFollower(UltraSonic* follower, bool& isLeft);

  void MakeWallParallel(UltraSonic* follower);

  ErrorCode ClearObstacle();

  bool isObsticalDetected();
  
  // Control loop to make sure driving straight
  void UpdateSpeed(float wallDist, bool isLeft);

  void ResetSpeed();

private: /* DATA */

  Motor L;            // Left motor
  Motor R;            // Right motor
  UltraSonicArray US; // Ultrasonic array on robot              
  float wheelbase;    // Distance between wheels

  // Control parameters
  float targetDist = 100.f;  // Target distance to follow wall
  float kp = 0.3f;          // Proportional gain
  float kd = 7.f;          // Differential gain
  
  // Derivative Estimate
  float prevDist;
  unsigned long prevTime;

  // Control Values
  int rSpeed;              // Right motor speed
  int lSpeed;              // Left motor speed

  ErrorCode m_err = OK;
};
