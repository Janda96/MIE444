#include "Modules.h"

// Standard Includes

// Libraries
#include <SparkFun_TB6612.h>

// Custom Includes
#include "Pins.h"

// Motors
Motor leftMotor = Motor(AIN1, AIN2, PWMA, 1, STBY);
Motor rightMotor = Motor(BIN1, BIN2, PWMB, 1, STBY);

// Ultrasoinc Sensors
UltraSonic US_Front(US2_T, US2_E);
UltraSonic US_Back(US4_T, US4_E);
UltraSonic US_Right(US3_T, US3_E);
UltraSonic US_Left(US1_T, US1_E);

// IR Sensors
IR IR_FR(IRD1);
IR IR_FL(IRD2);
IR IR_BL(IRD3);
IR IR_BR(IRD4);

// Ultrasonic Sensor Array
UltraSonicArray US = {US_Front, US_Back, US_Right, US_Left};

// IR Sensor Array
IRArray IRA = {IR_FR, IR_FL, IR_BR, IR_BL};

// Drivetrain
DriveTrain chasis(leftMotor, rightMotor, US, 10);
