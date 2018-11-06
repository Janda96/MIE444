#pragma once
// Standard Includes
#include <SparkFun_TB6612.h>

// Custom Includes
#include "Types.h"
#include "Actuators.h"

class DriveTrain
{
public:

  void Drive(char vel, Direction d);

  void Drive(char vel, int dist, Direction d);

  void Stop();

  void Turn(float angle);

  DriveTrain(Motor L, Motor R, float rad);

private:

  // Control loop to make sure driving straight
  void DriveStraight();

private: /* DATA */
  
  Motor L;
  Motor R;
  float rad;
};

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

class IRDist : private Sensor
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
