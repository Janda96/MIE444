#pragma once
// Standard Includes
#include <SparkFun_TB6612.h>

// Custom Includes
#include "Types.h"
#include "IO.h"

class UltraSonic
{
public:

  float getDist();

  UltraSonic(unsigned trigPin, unsigned echoPin);
  
private:

  Input echo;
  Output trig;
};

// Ultrasonic array on robot
struct UltraSonicArray
{
  UltraSonic* F;   // Front facing ultrasonic sensor
  UltraSonic* B;   // Back facing ultrasonic sensor
  UltraSonic* R;   // Right facing ultrasonic sensor
  UltraSonic* L;   // Left facing ultrasonic sensor
};

class IR : private Input
{
public:

  IR(unsigned pinInd);
  
private:

};

class IRDist : private Input
{
public:
  
private:

};

template<typename sensorType>
struct SensorAndPos
{
  sensorType Sensor;
  point pos;
};
