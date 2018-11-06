
// Custom Libraries
#include <SparkFun_TB6612.h>

// Standard Includes
#include "Sensors.h"
#include "Actuators.h"

// Custom Includes
#include "Types.h"

// --------------------
// Pin Assignements

// IR
#define IRD1 22
#define IRD2 23
#define IRD3 24
#define IRD4 25
#define IRA1 1
#define IRA2 2
#define IRA3 3
#define IRA4 4

// IR Distance
#define IR_5 5

// Ultrasonic
#define US1_T 26
#define US2_T 27
#define US3_T 28
#define US4_T 29
#define US1_E 30
#define US2_E 31
#define US3_E 32
#define US4_E 33

// MOTORS
#define AIN1 10
#define BIN1 9
#define AIN2 11
#define BIN2 8
#define PWMA 12
#define PWMB 7
#define STBY 13

// Servo
#define SERVO_PWM 5

// Battery Sensor
#define BAT_SENSE 0

// --------------------

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
