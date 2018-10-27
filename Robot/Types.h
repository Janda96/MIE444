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
  Forward,
  Backward
};

struct point
{
  float x;
  float y;
};
