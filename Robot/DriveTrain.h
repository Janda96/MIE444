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

  ErrorCode FollowWall(UltraSonic& follower, bool isLeft);

  ErrorCode LookFor(UltraSonic& follower);

  ErrorCode LostWall(bool isLeft);
  
  void Drive(int vel, Direction d);

  void Turn(float angle);

  void Stop();

  void set(int vel, bool isLeft);

  DriveTrain(Motor L, Motor R, UltraSonicArray US, float wheelbase);

  void MakeWallParallel(UltraSonic* follower, float searchWindowAngle);

private:

  ErrorCode ClearObstacle();

  bool isObsticalDetected();

  void AvoidWall();

  void AvoidWall(UltraSonic& DistSensor, bool isLeft);
  
  // Control loop to make sure driving straight
  void UpdateSpeed(float wallDist, bool isLeft);

private: /* DATA */
  
  // DriveTrain Modules
  Motor L;            // Left motor
  Motor R;            // Right motor
  UltraSonicArray US; // Ultrasonic array on robot    

  // DriveTrain Parameters
  float wheelbase;    // Distance between wheels

  // Control parameters
  float targetDist = 65.f;  // Target distance to follow wall
  float kp = 0.5f;          // Proportional gain
  float kd = -250.f;        // Differential gain

  // Derivative Estimate
  float prevDist;
  unsigned long prevTime;

  ErrorCode m_err = OK;
};
