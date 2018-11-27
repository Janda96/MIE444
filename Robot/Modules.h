#pragma once

// Standard Includes
#include <Arduino.h>
#include <LiquidCrystal.h>
#include <Servo.h>

// Custom Includes
#include "DriveTrain.h"
#include "Sensors.h"

// Chasis
extern DriveTrain chasis;

// Ultrasonic Sensor Array
extern UltraSonicArray US;

// IR Sensor Array
extern IRArray IRA;

// IR Distance Sensor
extern IRDist IRD;

// Servo
extern Servo MyServo;
