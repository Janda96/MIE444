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
