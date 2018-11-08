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
};

enum Direction
{
  Forward = -1,
  Backward = 1
};

struct point
{
  float x;
  float y;
};
