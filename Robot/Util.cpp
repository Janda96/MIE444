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
  int rx_byte;
  
  union encoderVals enc; 

  // Read all 12 bytes seperately
    for (auto i = 0; i < 12; ++i)
  {
    // wait for serial byte to come in
    while(Serial1.available() == 0);

    // Read serial byte
    rx_byte = Serial1.read();

    // Cast to char
    // and take the top byte of the int
    enc.raw[i] = (char) rx_byte;
  }
  
  //Get rid of the end byte
  while(Serial1.available() == 0);
  Serial1.read();
  Serial1.flush();

  // Cast to output variables
  x = enc.vals[0];
  y = enc.vals[1];
  angle = enc.vals[2];

}
