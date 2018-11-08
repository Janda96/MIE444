
// Custom Libraries
#include <SparkFun_TB6612.h>

// Standard Includes
#include <Servo.h>

// Custom Includes
#include "DriveTrain.h"
#include "Sensors.h"
#include "Pins.h"
#include "IO.h"
#include "Types.h"

// Motors
Motor leftMotor = Motor(AIN1, AIN2, PWMA, 1, STBY);
Motor rightMotor = Motor(BIN1, BIN2, PWMB, 1, STBY);

// Ultrasoinc Sensors
UltraSonic US_Front(US1_T, US1_E);
UltraSonic US_Back(US2_T, US2_E);
UltraSonic US_Right(US3_T, US3_E);
UltraSonic US_Left(US4_T, US4_E);

// Ultrasonic Sensor Array
UltraSonicArray US;
//US.F = &US_Front;
//US.B = &US_Back;
//US.R = &US_Right;
//US.L = &US_Left;

// Drivetrain
DriveTrain chasis(leftMotor, rightMotor, US, 10);

// Error code
ErrorCode err = OK;

void setup() 
{
  Serial.begin(9600);
}

void loop() 
{

  err = chasis.Drive();
  if (err == ObstacleDetected)
  {
    chasis.Turn(90.f);
  }
  else if (err == Blocked)
  {
    while (true);
  }
}
