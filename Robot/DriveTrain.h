#pragma once
// Standard Includes
#include <SparkFun_TB6612.h>

// Custom Includes
#include "Types.h"
#include "IO.h"
#include "Sensors.h"
#include "Util.h"

class DriveTrain
{
public:

  ErrorCode FollowWall(UltraSonic& follower, bool isLeft);

  ErrorCode LookFor(UltraSonic& follower);

  ErrorCode LostWall(bool isLeft);
  
  void Drive(int vel, Direction d);

  void DriveIntoWall(int vel, bool turnLeft);

  void Turn(float angle);

  void Stop();

  DriveTrain(Motor L, Motor R, UltraSonicArray US, float wheelbase);

  Orientation getLook();

  ErrorCode ClearObstacle();

private:

  bool isObsticalDetected();
  
  // Control loop to make sure driving straight
  void UpdateSpeed(float wallDist, bool isLeft);

  void updateOrientation(float angle);

private: /* DATA */
  
  // DriveTrain Modules
  Motor L;            // Left motor
  Motor R;            // Right motor
  UltraSonicArray US; // Ultrasonic array on robot    

  // Control parameters
  float targetDist = 65.f;  // Target distance to follow wall
  float kp = 0.3f;          // Proportional gain
  float kd = -250.f;        // Differential gain

  // Derivative Estimate
  float prevDist = targetDist;
  unsigned long prevTime = 0;

  // Orientation
  Point Look;

  ErrorCode m_err = OK;
};
