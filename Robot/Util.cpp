#include "Modules.h"

// Standard Includes

// Libraries
#include <SparkFun_TB6612.h>

// Custom Includes
#include "Pins.h"

// LCD
LiquidCrystal lcd(LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

bool fuzzyComp(float a, float b)
{
  return abs(a - b) < 0.1f;
}

Orientation getOrientation(Point L)
{
  // Left
  if (fuzzyComp(L.x, -1.f) && fuzzyComp(L.y, 0.f))
  {
    return Left;
  }

  // Right
  if (fuzzyComp(L.x, 1.f) && fuzzyComp(L.y, 0.f))
  {
    return Right;
  }

  // Up
  if (fuzzyComp(L.x, 0.f) && fuzzyComp(L.y, 1.f))
  {
    return Up;
  }
  
  // Down
  if (fuzzyComp(L.x, 0.f) && fuzzyComp(L.y, -1.f))
  {
    return Down;
  }

  // Default 
  // [NOTE: May need to change to error value]
  return Left;
}

void ReadEncoder(double& x, double& y, double& angle)
{
  Serial1.print('a');
  
  union encoderVals enc; 
  Serial1.readBytes(enc.raw, 12);    

  x = enc.vals[0];
  y = enc.vals[1];
  angle = enc.vals[2];

  for (auto i = 0; i < 12; ++i)
  {
    Serial.print(enc.raw[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
}
