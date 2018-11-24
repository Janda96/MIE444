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
  return (a - b) < 0.01f;
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
