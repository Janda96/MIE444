#include "DriveTrain.h"

// Standard Includes
#include <SparkFun_TB6612.h>

// Custom Includes
#include "Arduino.h"
#include "Util.h"

// Preprocessor definitions
#define DEG2RAD 3.14f / 180.f
#define RAD2DEG 180.f / 3.14f

// Motor Related
#define MAX_SPEED 150
#define MOTOR_CALIB 0.95f
#define DEFAULT_SPEED 100
#define DEFAULT_SPEED_R DEFAULT_SPEED
#define DEFAULT_SPEED_L MOTOR_CALIB * DEFAULT_SPEED_R

// Wall Avoidance Related
#define OBSTACLE_DIST 90.f
#define WALL_LOST_DIST 150.f

ErrorCode DriveTrain::FollowWall(UltraSonic& follower, bool isLeft)
{
    Serial3.println("Following Wall");

    double x, y, angle, distSinceLoc;
    float wallDist = 0.f;
    while (!isObsticalDetected())
    {
      ReadEncoder(x, y, angle);
      distSinceLoc = sqrt(sq(x) + sq(y));

      // Send localization data to matlab every 3 in
      if (distSinceLoc >= 7.62f)
      {
        // Reset encoder values
        Serial1.print('b');

        // Send localization data
        TakeAndSendLocMeasurement();
      }

      // Control
      wallDist = follower.getDist();
      UpdateSpeed(wallDist, isLeft);

      if (wallDist > WALL_LOST_DIST)
      {
        Stop();
        return WallDisapeared;
      }
    }
    Stop();
    return ObstacleDetected;
}

ErrorCode DriveTrain::LookFor(UltraSonic& follower)
{
    Serial3.println("Looking for wall");

    // Initialize encoder outputs
    double x, y, angle, distSinceLoc;

    // Drive until a wall is found
    float dist = follower.getDist();
    while (dist > WALL_LOST_DIST)
    {
      // Read the encoder and get the distance since last reset
      ReadEncoder(x, y, angle);
      double distSinceLoc = sqrt(sq(x) + sq(y));

      // Send localization data to matlab every 3 in
      if (distSinceLoc >= 7.62)
      {
        // Reset encoder values
        Serial1.print('b');

        // Send localization data
        TakeAndSendLocMeasurement();
      }

      // Drive forward
      Drive(DEFAULT_SPEED, Forward);

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

ErrorCode DriveTrain::LostWall(bool isLeft)
{
    // Stop for a second
    Stop();
    delay(1000);
    Serial3.println("Lost Wall");

    // Drive forward to clear the wall
    Drive(DEFAULT_SPEED, Forward);
    delay(500);
    Stop();

    // Turn to the open direction
    isLeft ? Turn(90) : Turn(-90);

    // Drive forward to find a new wall
    // So long as no obstacle is found
    Drive(DEFAULT_SPEED, Forward);
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

ErrorCode DriveTrain::ClearObstacle()
{
    Serial3.println("Clearing Obstical");

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
  //speedUpdate = min(speedUpdate, 12);
  //speedUpdate = max(speedUpdate, -12);

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

void DriveTrain::DriveIntoWall(int vel, bool turnLeft)
{
  Serial3.println("Driving into wall");

  // Drive forward until wall is hit
  Drive(vel, Forward);
  while(!isObsticalDetected());

  // Stop and turn left or right accordinly
  Stop();
  turnLeft ? Turn(-90.f) : Turn(90.f);
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

void DriveTrain::Turn(float angle)
{
  Serial3.println("Turning");

  static int turnSpeed = 100;

  angle = angle * DEG2RAD;

  // Update orientation
  updateOrientation(angle);

  // Find the direction the wheels have to turn
  int lSpeed = angle > 0 ? turnSpeed : -1 * turnSpeed;
  int rSpeed = angle > 0 ? -1 * turnSpeed : turnSpeed;

  float DelayGain = angle > 0 ? 380.f : 380.f;

  // Turn loop
  L.drive(lSpeed);  // NOTE: Might need to adjust speeds for specific sides
  R.drive(rSpeed);

  // Timebased turn
  unsigned timeDelay = static_cast<unsigned>(abs(angle) * DelayGain);
  delay(timeDelay);

  L.brake();
  R.brake();
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

void DriveTrain::TakeAndSendLocMeasurement()
{
  char cLook = getMappedOrientation(getLook());
  char cUSReading = getMappedUSReadings(US);

  SendLocMeasurement(cUSReading, cLook);
}

Orientation DriveTrain::getLook()
{
  return getOrientation(Look);
}

bool DriveTrain::isObsticalDetected()
{
    return US.F.getDist() < OBSTACLE_DIST;
}

void DriveTrain::Stop()
{
  brake(L, R);
  Serial3.println("Stopped!");
}

DriveTrain::DriveTrain(Motor L, Motor R, UltraSonicArray US) :
L(L),
R(R),
US(US)
{
  // Starting orientation is to the left
  Look.x = -1.f;
  Look.y = 0.f;
}
