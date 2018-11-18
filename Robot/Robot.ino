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
  chasis.Turn(90.f);
}
