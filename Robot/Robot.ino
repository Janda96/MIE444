
// Custom Libraries
#include <SparkFun_TB6612.h>

// Standard Includes
#include <Servo.h>
#include <LiquidCrystal.h>

// Custom Includes
#include "DriveTrain.h"
#include "Sensors.h"
#include "Pins.h"
#include "IO.h"
#include "Types.h"

// LCD
LiquidCrystal lcd(LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

// Motors
Motor leftMotor = Motor(AIN1, AIN2, PWMA, 1, STBY);
Motor rightMotor = Motor(BIN1, BIN2, PWMB, 1, STBY);

// Ultrasoinc Sensors
UltraSonic US_Front(US2_T, US2_E);
UltraSonic US_Back(US4_T, US4_E);
UltraSonic US_Right(US3_T, US3_E);
UltraSonic US_Left(US1_T, US1_E);

// Ultrasonic Sensor Array
UltraSonicArray US = {US_Front, US_Back, US_Right, US_Left};

// Drivetrain
DriveTrain chasis(leftMotor, rightMotor, US, 10);

// Error code
ErrorCode err = OK;

void setup() 
{
  lcd.begin(16, 2);
  Serial.begin(9600);
}

void loop() 
{
  err = chasis.Drive();
  if (err == Blocked)
  {
    lcd.print("I'm Stuck!!");
    while (true);
  }
  while (true);
}
