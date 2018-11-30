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

  // Core high level maze navigation functions
  ErrorCode FollowWall(bool isLeft);

  ErrorCode LookFor(bool isLeft);

  ErrorCode LostWall(bool isLeft);

  ErrorCode ClearObstacle();

  void DriveIntoWall(int vel, bool turnLeft);

  // Low level navigation functions
  void Drive(int vel, Direction d);

  void Turn(float angle);

  void Stop();

  // Accessor Functions
  Orientation getLook();

  void setLook(Orientation look);

  // Constructor
  DriveTrain(Motor L, Motor R, UltraSonicArray US);

private:

  // Control loop to make sure driving straight
  void UpdateSpeed(float wallDist, bool isLeft);

  void DisplayHeading();

  void TakeAndSendLocMeasurement();

  void updateOrientation(float angle);

  bool isObsticalDetected();

private: /* DATA */
  
  // DriveTrain Modules
  Motor L, R;            // Left and Right motors
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

  // Error code
  ErrorCode m_err = OK;
};
