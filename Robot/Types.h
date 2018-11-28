#pragma once
// Standard Includes
#include <Arduino.h>

// Custom Includes

// Wall Avoidance Related
#define OBSTACLE_DIST 90.f
#define WALL_LOST_DIST 150.f

// TURNING
#define RIGHT -90.f
#define LEFT 90.f
#define ABOUT_FACE 180.f

// WAll
#define RIGHT_WALL false
#define LEFT_WALL true

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

enum class Dir
{
  Front = 0,
  Back = 1,
  Left = 2,
  Right = 3,
};

union encoderVals
{
  double vals[3];
  char raw[12];
};
