#pragma once
// Standard Includes

// Custom Includes
#include "Types.h"

class Sensor
{
public:

  int readValue();

  Sensor(unsigned pinInd, bool isAnalog);

private: /* DATA */

  // Pin info
  unsigned pinInd;
  bool isAnalog;
};

// NOTE: Add reading to distance mapping as input!
class UltraSonic : private Sensor
{
public:

  UltraSonic(unsigned pinInd);
  
private:

};

class IR : private Sensor
{
public:

  IR(unsigned pinInd);
  
private:

};

class IRDistance : private Sensor
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
