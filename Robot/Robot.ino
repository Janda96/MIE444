// Custom Libraries

// Standard Includes
#include <Servo.h>
#include <Wire.h>

// Custom Includes
#include "Types.h"
#include "Util.h"
#include "Modules.h"
#include "Pins.h"
#include "Algos.h"

// Error code
ErrorCode errMain = OK;

// Drop Off Zone index

void setup()
{
  lcd.begin(20, 4);
  Wire.begin();
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial3.begin(9600);

  // Setup servo and center
  MyServo.attach(SERVO_PWM);
  MyServo.write(150.f);

  // Compass calibration 
  // (Uncomment when needed, only has to be done once and takes 15 seconds)
  // compassCalibrate();

  // Compas setup
  Compass.SetDeclination(-10, 26, 'W');  
  Compass.SetSamplingMode(COMPASS_SINGLE);
  
  // Sensitivity (higher is less sensitive, values are 088, 130 (default), 190, 250, 400, 470, 560, 810
  Compass.SetScale(COMPASS_SCALE_250);  
  Compass.SetOrientation(COMPASS_HORIZONTAL_X_NORTH);

  Serial3.println("I'M ALIVE!!!");
}

void loop()
{
  
  lcd.print(ReadBat());

  // Read in which drop off zone to go to
  int DZInd = -10;
  while(DZInd < 0 || DZInd > 3)
  {
     Serial3.println("Please Select a drop off zone");
     while(!Serial3.available());
     DZInd = Serial3.read();
     DZInd -= 48;  // Need to offset for some reason
  }
  
//  chasis.Turn(360.f);
//  delay(1000);
//  return;

//  // For calibrating left heading
//  while(true)
//  {
//      PrintHeading();
//  }
  
  // Make sure looking left and set starting orientation left
  // TurnLeftWithRandomOrientation();
  chasis.setLook(Left);

  // Display in loading zone
  lcd.clear();
  lcd.print("GOING TO LZ");
  
  // Get to Lz, pickup block and go to DZ
  GetToLZ();

  // Display in loading zone
  lcd.clear();
  lcd.print("IN LOADING ZONE");
  delay(1000);

  lcd.clear();
  lcd.print("TURNING TOWARDS BLOCK");

  // Roughly turn towards block
  auto look = chasis.getLook();
  look == Up ? chasis.Turn(-135.f) : chasis.Turn(135.f);

  lcd.clear();
  lcd.print("DRIVING TOWARDS BLOCK");

  // Drive towards block
  chasis.Drive(100.f, Forward);
  delay(1000);
  chasis.Stop();

  // Pickup block
  BlockPickup();

  lcd.clear();
  lcd.print("TURNING TOWARDS WALL");
  
  // Turn towards correct wall
  // to get to desired drop off zone
  bool turnLeft = DZInd < 3;
  turnLeft ? chasis.Turn(135.f) : chasis.Turn(-135.f);

  lcd.clear();
  lcd.print("DRIVING INTO WALL");

  // Drive into the wall and turn
  // Towards the drop off zone
  chasis.DriveIntoWall(100.f, !turnLeft);

  turnLeft ? chasis.setLook(Right) : chasis.setLook(Down);

  lcd.clear();
  lcd.print("GOING TO DZ");

  // Get to the drop off zone
  LzToDz[DZInd]();

  // Display confirmation that in drop off zone
  lcd.clear();
  lcd.print("IN DROP-OFF ZONE");
  delay(1000);

  // Drop Block off
  BlockDropoff();

  // Reverse off block
  chasis.Drive(100.f, Backward);
  delay(1000);
  chasis.Stop();

  // Display confirmation that block is dropped off
  lcd.clear();
  lcd.print("BLOCK DROPPED OFF");
  delay(1000);
  
  // STOP
  DZInd = -10;
  while(DZInd != 0)
  {
     while(!Serial3.available());
     DZInd = Serial3.read();
     DZInd -= 48;  // Need to offset for some reason
  }
}
