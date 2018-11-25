#pragma once
// Standard Includes
#include <Arduino.h>

// Custom Includes

enum ErrorCode
{
  OK = 0,
  InvalidInput = -1,
  ObstacleDetected = -2,
  Blocked = -3,
  WallDisapeared = -4,
};

enum Direction
{
  Forward = -1,
  Backward = 1
};

enum Orientation
{
  Up = 0,
  Down = 1,
  Left = 2,
  Right = 3,
};

struct Point
{
   float x;
   float y;
};

struct Pose
{
   float x;
   float y;
   Orientation O;
};

union encoderVals
{
  double vals[3];
  char raw[12];
};
