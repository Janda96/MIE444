// Custom Libraries

// Standard Includes
#include <Servo.h>

// Custom Includes
#include "Types.h"
#include "Util.h"
#include "Modules.h"
#include "Pins.h"

// Error code
ErrorCode errMain = OK;

void setup() 
{
  lcd.begin(20, 4);
  Serial3.begin(9600);
}

void loop() 
{ 
  // For drop off zone 1,2,3
  // Place robot pointing right along top wall in loading zone
  // For drop off zone 4
  // Place robot pointing down along left wall in loading zone
  
  // Get from loading zone to drop off zone
  LzToDz1();
  // LzToDz2();
  // LzToDz3();
  // LzToDz4();
  
  chasis.Stop();
  while (true);
}
