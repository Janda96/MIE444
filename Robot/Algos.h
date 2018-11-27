#pragma once

// Standard Includes

// Custom Includes

//----------MAZE-------------

typedef void(*voidFuncType)();

// Directions to loading zone 
void GetToLZ();

// Directions from loading zone to drop off zone
void LzToDz1();
void LzToDz2();
void LzToDz3();
void LzToDz4();

// Array map of loading zone to drop off zone
extern voidFuncType LzToDz[4];

// Localization data
void Localize();
void SendLocMeasurements();

// Utility functions
bool inLoadingZone();

// Crystal remote control
void RemoteControl();

// Block functions
void BlockPickup();
void BlockDropoff();

//------BLOCK PICKUP-----

bool isBlockDetected();

void TurnTowardsBlock(float searchWindowAngle);
