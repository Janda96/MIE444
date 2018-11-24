// Standard Includes

// Custom Includes

//----------MAZE-------------

// Directions to loading zone 
void GetToLZ();

// Directions from loading zone to drop off zone
void LzToDz1();
void LzToDz2();
void LzToDz3();
void LzToDz4();

// Localization data
void Localize();

// Utility functions
bool isLoadingZone();

//------BLOCK PICKUP-----

bool isBlockDetected();

void TurnTowardsBlock(float searchWindowAngle);
