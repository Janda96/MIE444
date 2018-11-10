#include "DriveTrain.h"

// Standard Includes
#include <SparkFun_TB6612.h>

// Custom Includes
#include "Arduino.h"
#include "Modules.h"

// Preprocessor definitions
#define DEG2RAD 180.f / 3.14f

// Motor Related
#define MAX_SPEED 150
#define MOTOR_CALIB 0.95f
#define DEFAULT_SPEED_R 100
#define DEFAULT_SPEED_L MOTOR_CALIB * DEFAULT_SPEED_R

// Wall Avoidance Related
#define OBSTACLE_DIST 75.f
#define AVOID_DIST 100.f
#define WALL_AVOID_DIST 40.f
#define WALL_DETECT_DIST 300.f

DriveTrain::DriveTrain(Motor L, Motor R, UltraSonicArray US, float wheelbase) :
L(L),
R(R),
US(US),
wheelbase(wheelbase)
{
}

ErrorCode DriveTrain::Drive()
{
    while (true)
    {
      // [0] -> Clear any obstacles
      while (isObsticalDetected())
      {
        m_err = ClearObstacle();
        if (m_err != OK)
        {
            Stop();
            return m_err;
        }
      }
      
      // [1] -> Find an ultrasonic sensor to follow
      bool isLeft;
      UltraSonic* follower;
      m_err = FindFollower(follower, isLeft);
      if (m_err != OK)
      {
          Stop();
          return m_err;
      }
  
      // [2] -> Follow wall so long as no obstacle detected
      float wallDist;
      while (!isObsticalDetected())
      {
          AvoidWall();

          wallDist = follower->getDist();
          Serial3.println(wallDist);
          if (wallDist > WALL_DETECT_DIST)
          {
              Serial3.println("LOST WALL!!");
              Stop();
              delay(1000);
              Drive(DEFAULT_SPEED_R, Forward);
              delay(500);
              Stop();
              if (isLeft)
              {
                Turn(90);
              }
              else
              {
                Turn(-90);
              }
              Drive(DEFAULT_SPEED_R, Forward);
              for (auto i = 0; i < 40; ++i)
              {
                if (isObsticalDetected())
                {
                  break;
                }
                delay(10);
              }
              
              break;
          }
          
          UpdateSpeed(wallDist, isLeft);
          delay(100); // Might be unnecessary
      }
    }
    
    return m_err;
}

ErrorCode DriveTrain::FindFollower(UltraSonic*& follower, bool& isLeft)
{
    float lDist = US.L.getDist();
    float rDist = US.R.getDist();

    // Drive until a wall is found
    while (lDist > WALL_DETECT_DIST && rDist > WALL_DETECT_DIST)
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
        lDist = US.L.getDist();
        rDist = US.R.getDist();
    }
    
    if (rDist < WALL_DETECT_DIST)
    {
      isLeft = false;
      follower = &(US.R);
    }
    else
    {
      isLeft = true;
      follower = &(US.L);
    }

    // Make robot parrallel to wall
    Serial3.println("FOUND FOLLOWER");  
    return OK;
}

void DriveTrain::MakeWallParallel(UltraSonic* follower)
{
    Turn(-20);
    L.drive(60);
    R.drive(-60);

    float dist = follower->getDist();
    float currDist;

    delay(200);
    while (true)
    {
      currDist = follower->getDist();
      if (currDist > dist)
      {
        break;
      }
      
      dist = currDist;
    }
    
    // Turn(-15);
    Stop();
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
        if (lDist > AVOID_DIST)
        {
          Turn(90.f);
        }
        else if (rDist > AVOID_DIST)
        {
          Turn(-90.f);
        }
        else if (bDist > AVOID_DIST)
        {
          Turn(180.f);
        }
        else
        {
          return Blocked;
        }
    }

    Serial3.println("CLEARED OBSTACLE");
    return OK;
}

bool DriveTrain::isObsticalDetected()
{
    return US.F.getDist() < OBSTACLE_DIST;
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

void DriveTrain::Drive(int vel, Direction d)
{
  if (isObsticalDetected())
  {
    ClearObstacle();
  }

  R.drive(d * vel);
  L.drive(d * MOTOR_CALIB * vel);
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

  // Find the direction the wheels have to turn
  int lSpeed = angle > 0 ? turnSpeed : -1 * turnSpeed;
  int rSpeed = angle > 0 ? -1 * turnSpeed : turnSpeed;

  float DelayGain = angle > 0 ? 0.13f : 0.13f;

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

void DriveTrain::Stop()
{
  brake(L, R);
}
