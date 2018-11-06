
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

// Ultrasonic
UltraSonic US1(US1_T, US1_E);
UltraSonic US2(US2_T, US2_E);
UltraSonic US3(US3_T, US3_E);
UltraSonic US4(US4_T, US4_E);

void setup() {
  
  Serial.begin(9600);              //  setup serial
}

void loop() {
  
  
}
