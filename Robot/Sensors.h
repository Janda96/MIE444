#pragma once
// Standard Includes

// Custom Includes
#include "Types.h"
#include "Actuators.h"

class Sensor
{
public:

  int readValue();

  unsigned long timePulse(int value);

  Sensor(unsigned pinInd, bool isAnalog);

private: /* DATA */

  // Pin info
  unsigned pinInd;
  bool isAnalog;
};

class UltraSonic
{
public:

  float getDist();

  UltraSonic(unsigned trigPin, unsigned echoPin);
  
private:

  Sensor echo;
  Actuator trig;
  
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
