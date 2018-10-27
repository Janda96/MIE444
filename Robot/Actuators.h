#pragma once
// Standard Includes
#include <Servo.h>

// Custom Includes
#include "Types.h"

// Servo Positions
static const unsigned NeutralPos = 90;
static const unsigned ReleasePos = 180;
static const unsigned PickupPose = 0;

class Actuator
{
public:

  void set(int val);

  Actuator(unsigned pinInd, bool isPWM);

private: /* DATA */

  // Pin info
  unsigned pinInd;
  bool isPWM;
  
};

class Motor : private Actuator
{
public:
  
  void Drive(int vecolcity);

  void Drive(int vecolcity, int distance);

  void Stop();

  void SetDirection(Direction d);

  Motor(unsigned pinInd);
  
private: /* DATA */
  
  Direction d = Forward;
  
};

class DriveTrain
{
public:

  void DriveStraight(int veclocity);

  void DriveStraight(int vecolcity, int distance);

  void Stop();

  void SetDirection(Direction d);

  void Turn(float angle);

  DriveTrain(unsigned LeftMotorPin, unsigned RightMotorPin);

private:

  // Control loop to make sure driving straight
  void DriveStraight();

private: /* DATA */
  
  Motor L;
  Motor R;

  Direction d;
};
