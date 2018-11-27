#include "Modules.h"

// Standard Includes

// Libraries
#include <SparkFun_TB6612.h>

// Custom Includes
#include "Pins.h"

// LCD
LiquidCrystal lcd(LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

// Localization Code
#define WALL_DIST 100.f

void SendLocMeasurement(char USReading, char look)
{
  Serial3.println(USReading);
  Serial3.println(look);
}

char getMappedOrientation(Orientation Look)
{
  switch (Look)
  {
    case Up:
      return 'U';
    case Down:
      return 'D';
    case Left:
      return 'L';
    case Right:
      return 'R';
    default:
      return 'L';
  }
}

char getMappedUSReadings(UltraSonicArray& US)
{
  bool isFrontWall = US.F.getDist() < WALL_DIST;
  bool isBackWall = US.B.getDist() < WALL_DIST;
  bool isRightWall = US.R.getDist() < WALL_DIST;
  bool isLeftWall = US.L.getDist() < WALL_DIST;

  int walls[4] = {isFrontWall, isBackWall, isRightWall, isLeftWall};
  
  int sumArr = sumIntArr(walls, 4);
  bool areOpposite = areWallsOpposite(walls);

  switch (sumArr)
  {
    case 0:
      return '0';
    case 1:
      return '1';
    case 3:
      return '3';
    case 2:
      if (areOpposite)
      {
        return '5';
      }
      else
      {
        return '2';
      }
    default:
      return '4';
  }
}

bool areWallsOpposite(int arr[4])
{
  if (arr[0] == 1 && arr[1] == 1)
  {
    return true;
  }
  else if (arr[2] == 1 && arr[3] == 1)
  {
    return true;
  }
  else
  {
    return false;
  }
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

int sumIntArr(int* arr, int sizeOfArr)
{
  int sum = 0;
  for (auto i = 0; i < sizeOfArr; ++i)
  {
    sum += arr[i];
  }
  return sum;
}

float ReadBat()
{
  return(map(analogRead(0), 0, 1024, 0, 15));
}

bool fuzzyComp(float a, float b)
{
  return abs(a - b) < 0.1f;
}
