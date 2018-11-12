// Custom Libraries

// Standard Includes
#include <Servo.h>

// Custom Includes
#include "Types.h"
#include "Util.h"
#include "Modules.h"
#include "Pins.h"

// Error code
ErrorCode err = OK;

void setup() 
{
  lcd.begin(20, 4);
  Serial3.begin(9600);
}

void loop() 
{
  lcd.println(analogRead(BAT_SENSE)*0.01465f);
  err = chasis.Drive();
  if (err == Blocked)
  {
    lcd.print("I'm Stuck!!");
    while (true);
  }
}
