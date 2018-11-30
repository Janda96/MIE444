#pragma once

// Standard Includes
#include <LiquidCrystal.h>

// Custom Includes
#include "Types.h"
#include "Sensors.h"
#include "DriveTrain.h"

// LCD
extern LiquidCrystal lcd;

Orientation getOrientation(Point L);

bool fuzzyComp(float a, float b);

void ReadEncoder(double& x, double& y, double& angle);

float ReadBat();

//--- Localization Functions ---

void SendLocMeasurement(char USReading, char look);

char getMappedUSReadings(UltraSonicArray& US);

char getMappedOrientation(Orientation Look);

bool areWallsOpposite(int* arr);

int sumIntArr(int* arr, int sizeOfArr);
