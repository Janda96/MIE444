#pragma once
// Standard Includes
#include <SparkFun_TB6612.h>

// Custom Includes
#include "NewPing.h"
#include "Types.h"
#include "IO.h"

class UltraSonic
{
public:

  float getDist();

  UltraSonic(unsigned trigPin, unsigned echoPin);
  
private:

  NewPing sonar;
};

class IR : private Input
{
public:

  float getBW();

  IR(unsigned pinInd);

};

class IRDist : private Input
{
public:

  float getDist();

  IRDist(unsigned pinInd);
};

// Ultrasonic array on robot
struct UltraSonicArray
{
  UltraSonic F;   // Front facing ultrasonic sensor
  UltraSonic B;   // Back facing ultrasonic sensor
  UltraSonic R;   // Right facing ultrasonic sensor
  UltraSonic L;   // Left facing ultrasonic sensor
};

// IR colour array on robot
struct IRArray
{
  IR FR;   // Front right facing IR sensor
  IR FL;   // Front left facing IR sensor
  IR BR;   // Back Right facing IR sensor
  IR BL;   // Back Left facing IR sensor
};
