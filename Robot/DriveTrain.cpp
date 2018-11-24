#include "DriveTrain.h"

// Standard Includes
#include <SparkFun_TB6612.h>

// Custom Includes
#include "Arduino.h"
#include "Modules.h"

// Preprocessor definitions
#define DEG2RAD 3.14f / 180.f
#define RAD2DEG 180.f / 3.14f

// Motor Related
#define MAX_SPEED 150
#define MOTOR_CALIB 0.95f
#define DEFAULT_SPEED_R 100
#define DEFAULT_SPEED_L MOTOR_CALIB * DEFAULT_SPEED_R

// Wall Avoidance Related
#define OBSTACLE_DIST 75.f
#define AVOID_DIST 100.f
#define WALL_AVOID_DIST 40.f
#define WALL_DETECT_DIST 100.f

ErrorCode DriveTrain::FollowWall(UltraSonic& follower, bool isLeft)
{
    float wallDist = 0.f;
    while (!isObsticalDetected())
    {
      // Avoid wall if gets too close
      AvoidWall();  

      // Control
      wallDist = follower.getDist();
      UpdateSpeed(wallDist, isLeft);

      if (wallDist > WALL_DETECT_DIST)
      {
        return WallDisapeared;
      }
      
      delay(100); // Might be unnecessary
    }
    return ObstacleDetected;
}

ErrorCode DriveTrain::LookFor(UltraSonic& follower)
{
    float dist = follower.getDist();

    // Drive until a wall is found
    while (dist > WALL_DETECT_DIST)
    {
        // Drive forward
        Drive(DEFAULT_SPEED_R, Forward);

        // Make sure robot doesnt hit anything
        if (isObsticalDetected())
        {
          Stop();
          m_err = ClearObstacle();
          if (m_err != OK)
          {
            return m_err;
          }
        }

        // Check distances
        dist = US.L.getDist();
    }
    
    Stop();
    return OK;
}

ErrorCode DriveTrain::ClearObstacle()
{
    // Take measurements about robots' surroundings
    float bDist = US.B.getDist();
    float lDist = US.L.getDist();
    float rDist = US.R.getDist();
    
    // Check if cannot go forward
    // Will need to turn to free spot in this case
    if (isObsticalDetected())
    {
        if (lDist > OBSTACLE_DIST)
        {
          Turn(90.f);
        }
        else if (rDist > OBSTACLE_DIST)
        {
          Turn(-90.f);
        }
        else if (bDist > OBSTACLE_DIST)
        {
          Turn(180.f);
        }
        else
        {
          return Blocked;
        }
    }
    return OK;
}

void DriveTrain::UpdateSpeed(float wallDist, bool isLeft)
{
  // Calculate the derivative component
  unsigned long currTime = millis();
  float d = (wallDist - prevDist) / static_cast<float>(currTime - prevTime);
  prevTime = currTime;
  prevDist = wallDist;
  
  // Calculate the proportion component
  float p = targetDist - wallDist;

  // Find speed update and limit
  float speedUpdate = kp * p + kd * d;
  speedUpdate = min(speedUpdate, 12);
  speedUpdate = max(speedUpdate, -12);
  
  // Update motor speeds based on control input
  float rSpeed, lSpeed;
  if (isLeft)
  {
      rSpeed = DEFAULT_SPEED_R - speedUpdate;
      lSpeed = DEFAULT_SPEED_L + speedUpdate;
  }
  else
  {
      rSpeed = DEFAULT_SPEED_R + speedUpdate;
      lSpeed = DEFAULT_SPEED_L - speedUpdate;
  }
  
  // Limit max speed
  rSpeed = min(rSpeed, MAX_SPEED);
  lSpeed = min(lSpeed, MAX_SPEED);

  // limit min speed
  rSpeed = max(rSpeed, 0);
  lSpeed = max(lSpeed, 0);

  L.drive(-lSpeed);
  R.drive(-rSpeed);
}

void DriveTrain::Turn(float angle)
{
  static int turnSpeed = 100;

  angle = angle * DEG2RAD;

  // Update orientation
  updateOrientation(angle);

  // Find the direction the wheels have to turn
  int lSpeed = angle > 0 ? turnSpeed : -1 * turnSpeed;
  int rSpeed = angle > 0 ? -1 * turnSpeed : turnSpeed;

  float DelayGain = angle > 0 ? 400.f : 400.f;

  float turnDist = wheelbase / 2.f * abs(angle);
  float currDist = 0.f;

  // Turn loop
  L.drive(lSpeed);  // NOTE: Might need to adjust speeds for specific sides
  R.drive(rSpeed);
  while( currDist < turnDist)
  {
    // Read sensors
    // currDist = 

    // Timebased turn
    unsigned timeDelay = static_cast<unsigned>(abs(angle) * DelayGain);
    delay(timeDelay);
    break;
  }
  
  L.brake();
  R.brake();
}

void DriveTrain::AvoidWall()
{
    AvoidWall(US.L, true);  // Avoid left wall
    AvoidWall(US.R, false); // Avoid right wall
}

void DriveTrain::AvoidWall(UltraSonic& DistSensor, bool isLeft)
{
    float dist = DistSensor.getDist();
    float angle = isLeft ? -5 : 5;
    if (dist < WALL_AVOID_DIST)
    {
        Stop();
        Turn(angle);
        Drive(DEFAULT_SPEED_R, Forward);
        delay(500);
        Stop();
    }
}

ErrorCode DriveTrain::LostWall(bool isLeft)
{
    Serial3.println("LOST WALL!!");

    // Stop for a second
    Stop();
    delay(1000);

    // Drive forward to clear the wall
    Drive(DEFAULT_SPEED_R, Forward);
    delay(500);
    Stop();

    // Turn to the open direction
    isLeft ? Turn(90) : Turn(-90);

    // Drive forward to find a new wall
    // So long as no obstacle is found
    Drive(DEFAULT_SPEED_R, Forward);
    for (auto i = 0; i < 40; ++i)
    {
      if (isObsticalDetected())
      {
        break;
      }
      delay(10);
    }
    
    return OK;
}

void DriveTrain::Drive(int vel, Direction d)
{
  if (isObsticalDetected())
  {
    ClearObstacle();
  }

  R.drive(d * vel);
  L.drive(d * MOTOR_CALIB * vel);
}

void DriveTrain::MakeWallParallel(UltraSonic* follower, float searchWindowAngle)
{
  static float angleIncrement = (2.f * searchWindowAngle) / 10.f;
  
  chasis.Turn(-1.f * searchWindowAngle);
 
  // Sweep to find block
  float minDist = 500.f;
  float minAngle = 0.f;
  float dist = 500.f;
  for (auto angle = -1.f * searchWindowAngle; angle < searchWindowAngle + 0.1f; angle += angleIncrement)
  {
    dist = follower->getDist();
    if (dist < minDist)
    {
        minAngle = angle;
        minDist = dist;
    }
    chasis.Turn(angleIncrement);
    delay(500);
  }
  
  chasis.Turn(minAngle - searchWindowAngle + 5);
}

void DriveTrain::updateOrientation(float angle)
{
  float c_th = cos(angle);
  float s_th = sin(angle);
  
  float x = Look.x;
  float y = Look.y;

  // Perform rotation
  Look.x = c_th * x - s_th * y;
  Look.y = s_th * x + c_th * y;

  // Remove truncation error
  Look.x = fuzzyComp(Look.x, 0.f) ? 0.f : Look.x;
  Look.x = fuzzyComp(Look.x, 1.f) ? 1.f : Look.x;
  Look.x = fuzzyComp(Look.x, -1.f) ? -1.f : Look.x;

  Look.y = fuzzyComp(Look.y, 0.f) ? 0.f : Look.y;
  Look.y = fuzzyComp(Look.y, 1.f) ? 1.f : Look.y;
  Look.y = fuzzyComp(Look.y, -1.f) ? -1.f : Look.y;
}

Orientation DriveTrain::getLook()
{
  return getOrientation(Look);
}

void DriveTrain::set(int vel, bool isLeft)
{
  isLeft ? L.drive(vel) : R.drive(vel);
}

bool DriveTrain::isObsticalDetected()
{
    return US.F.getDist() < OBSTACLE_DIST;
}

void DriveTrain::Stop()
{
  brake(L, R);
}

DriveTrain::DriveTrain(Motor L, Motor R, UltraSonicArray US, float wheelbase) :
L(L),
R(R),
US(US),
wheelbase(wheelbase)
{
  // Starting orientation is to the left
  Look.x = -1.f;
  Look.y = 0.f;
}
