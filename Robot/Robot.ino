// Custom Libraries

// Standard Includes
#include <Servo.h>

// Custom Includes
#include "Types.h"
#include "Util.h"
#include "Modules.h"
#include "Pins.h"
#include "Algos.h"

// Error code
ErrorCode errMain = OK;

void setup()
{
  lcd.begin(20, 4);

  Serial.begin(9600);
  Serial1.begin(9600);
  Serial3.begin(9600);

  Serial3.println("I'M ALIVE!!!");
}

int DZInd = 0;

void loop()
{
  lcd.print(ReadBat());

  // Get to Lz, pickup block and go to DZ
  GetToLZ();

  // Roughly turn towards block
  Serial3.println("TURNING TOWARDS BLOCK");
  auto look = chasis.getLook();
  look == Up ? chasis.Turn(-135.f) : chasis.Turn(135.f);

  // Drive towards block
  Serial3.println("DRIVING TOWARDS BLOCK");
  chasis.Drive(100.f, Forward);
  delay(1000);
  chasis.Stop();

  // Pickup block
  BlockPickup();

  // Turn towards correct wall
  // to get to desired drop off zone
  Serial3.println("TURNING TOWARDS WALL");
  bool turnLeft = DZInd < 3;
  turnLeft ? chasis.Turn(135.f) : chasis.Turn(-135.f);

  // Drive into the wall and turn
  // Towards the drop off zone
  Serial3.println("DRIVING INTO WALL");
  chasis.DriveIntoWall(100.f, turnLeft);

  // Get to the drop off zone
  Serial3.println("GOING TO DROP OFF ZONE");
  LzToDz[DZInd]();

  // Drop Block off
  BlockPickup();

  // Display confirmation that in drop off zone
  lcd.clear();
  lcd.print("IN DROP-OFF ZONE");
  delay(1000);

  // Reverse off block
  chasis.Drive(100.f, Backward);
  delay(1000);
  chasis.Stop();

  // Display confirmation that block is dropped off
  lcd.clear();
  lcd.print("BLOCK DROPPED OFF");
  delay(1000);
  
  // STOP
  while(true);
}
