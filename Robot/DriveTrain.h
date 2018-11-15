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

  ErrorCode FindFollower(UltraSonic*& follower, bool& isLeft);

  ErrorCode ClearObstacle();

  bool isObsticalDetected();

  void AvoidWall();

  void AvoidWall(UltraSonic& DistSensor, bool isLeft);

  void DealWithLostWall(bool isLeft);
  
  // Control loop to make sure driving straight
  void UpdateSpeed(float wallDist, bool isLeft);

  void MakeWallParallel(UltraSonic* follower);

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
