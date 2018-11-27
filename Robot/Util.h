#pragma once

// Standard Includes
#include <LiquidCrystal.h>

// Custom Includes
#include "Types.h"

// LCD
extern LiquidCrystal lcd;

Orientation getOrientation(Point L);

bool fuzzyComp(float a, float b);

void ReadEncoder(double& x, double& y, double& angle);

float ReadBat();
