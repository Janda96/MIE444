#pragma once

// Standard Includes
#include <LiquidCrystal.h>

// Custom Includes
#include "Types.h"

// LCD
extern LiquidCrystal lcd;

Orientation getOrientation(Point L);

bool fuzzyComp(float a, float b);
