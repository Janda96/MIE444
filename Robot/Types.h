#pragma once
// Standard Includes

// Custom Includes

enum ErrorCode
{
  OK = 0,
  InvalidInput = -1,
};

enum Direction
{
  Forward = 1,
  Backward = -1
};

struct point
{
  float x;
  float y;
};

struct DistSensorMap
{
  int minDist;
  int minDistVal;
  int maxDist;
  int maxDistVal;
};
