#include "DriveTrain.h"

// Standard Includes
#include <SparkFun_TB6612.h>

// Custom Includes
#include "Arduino.h"

DriveTrain::DriveTrain(Motor L, Motor R, float rad) :
L(L),
R(R),
rad(rad)
{
}

void DriveTrain::Drive(char vel, Direction d)
{
  L.drive(d * vel);
  R.drive(d * vel);
}

void DriveTrain::Drive(char vel, int dist, Direction d)
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

void DriveTrain::Turn(float angle)
{
  static int turnSpeed = 50;

  // Find the direction the wheels have to turn
  int lSpeed = angle > 0 ? turnSpeed : -1 * turnSpeed;
  int rSpeed = angle > 0 ? -1 * turnSpeed : turnSpeed;

  float dist = rad * abs(angle);
  float rightDist = 0.f;
  float leftDist = 0.f;

  // Turn loop
  L.drive(lSpeed);
  R.drive(rSpeed);
  while( rightDist < dist && leftDist < dist)
  {
    // Read sensors
    // rightDist = 
    // leftDist = 
    if (rightDist > dist) 
    {
      R.brake();
    }
    if (leftDist > dist)
    {
      L.brake();
    }

    delay(3000);
    break;
  }
  
  L.brake();
  R.brake();
}

void DriveTrain::Stop()
{
  brake(L, R);
}
