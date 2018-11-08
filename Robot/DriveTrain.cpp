#include "DriveTrain.h"

// Standard Includes
#include <SparkFun_TB6612.h>

// Custom Includes
#include "Arduino.h"

// Preprocessor definitions
#define MAX_SPEED 100
#define DEFAULT_SPEED 50
#define OBSTACLE_DIST 50.f
#define DEG2RAD 180.f / 3.14f

DriveTrain::DriveTrain(Motor L, Motor R, UltraSonicArray US, float wheelbase) :
L(L),
R(R),
US(US),
wheelbase(wheelbase)
{
  ResetSpeed();
}

ErrorCode DriveTrain::Drive()
{
    while (true)
    {
      // [0] -> Clear any obstacles
      m_err = ClearObstacle();
      if (m_err != OK)
      {
          Stop();
          return m_err;
      }
    
      // [1] -> Find an ultrasonic sensor to follow
      UltraSonic* follower;
      m_err = FindFollower(follower);
      if (m_err != OK)
      {
          Stop();
          return m_err;
      }
  
      // [2] -> Follow wall so long as no obstacle detected
      float wallDist;
      while (!isObsticalDetected())
      {
          wallDist = follower->getDist();
          if (wallDist > 10.f)
          {
              Stop();
              break;
          }
          
          UpdateSpeed(wallDist);
          delay(500); // Might be unnecessary
      }
    }
    
    return m_err;
}

ErrorCode DriveTrain::FindFollower(UltraSonic* follower)
{
    float lDist = US.L.getDist();
    float rDist = US.R.getDist();

    // Drive until a wall is found
    while (lDist > 50.f && rDist > 50.f)
    {
        // Drive forward
        Drive(DEFAULT_SPEED, Forward);
        delay(1000);

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
        lDist = US.L.getDist();
        rDist = US.R.getDist();
    }
    
    if (rDist < 50.f)
    {
      follower = &(US.R);
    }
    else if(lDist < 50.f)
    {
      follower = &(US.L);
    }

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
        if (lDist > 50.f)
        {
          Turn(-90.f);
        }
        else if (rDist > 50.f)
        {
          Turn(90.f);
        }
        else if (bDist > 50.f)
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

bool DriveTrain::isObsticalDetected()
{
    return US.F.getDist() < OBSTACLE_DIST;
}

void DriveTrain::Drive(int vel, Direction d)
{
  L.drive(d * vel);
  R.drive(d * vel);
}

void DriveTrain::Drive(int vel, int dist, Direction d)
{
  Drive(vel, d);
  float currDist = 0.f;
  while (currDist < dist)
  {
    // Update distance
    // currDist = 
    
    delay(5000);
    break;
  }
  Stop();
}

void DriveTrain::UpdateSpeed(float wallDist)
{
  // Calculate the derivative component
  float currTime = millis();
  float d = (wallDist - prevDist) / (currTime - prevTime);
  prevTime = currTime;
  
  // Calculate the proportion component
  float p = targetDist - wallDist;

  // Update motor speeds based on control input
  float speedUpdate = kp * p + kd * d;
  rSpeed += speedUpdate;
  lSpeed -= speedUpdate;

  // Limit max speed
  rSpeed = max(rSpeed, MAX_SPEED);
  lSpeed = max(lSpeed, MAX_SPEED);

  L.drive(lSpeed);
  R.drive(rSpeed);
}

void DriveTrain::ResetSpeed()
{
  rSpeed = DEFAULT_SPEED;
  lSpeed = DEFAULT_SPEED;
}

void DriveTrain::Turn(float angle)
{
  static int turnSpeed = 50;

  angle = angle * DEG2RAD;

  // Find the direction the wheels have to turn
  int lSpeed = angle > 0 ? turnSpeed : -1 * turnSpeed;
  int rSpeed = angle > 0 ? -1 * turnSpeed : turnSpeed;

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
    unsigned timeDelay = static_cast<unsigned>(angle * 1000);
    delay(timeDelay);
    break;
  }
  
  L.brake();
  R.brake();
}

void DriveTrain::Stop()
{
  brake(L, R);
}
