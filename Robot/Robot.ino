// Custom Libraries

// Standard Includes
#include <Servo.h>

// Custom Includes
#include "Types.h"
#include "Util.h"
#include "Modules.h"
#include "Pins.h"
#include "Algos.h"

// Error code
ErrorCode errMain = OK;

void setup() 
{
  lcd.begin(20, 4);
  Serial3.begin(9600);
  Serial.begin(9600);
  MyServo.attach(SERVO_PWM);  
}

void loop() 
{ 
  // For drop off zone 1,2,3
  // Place robot pointing right along top wall in loading zone
  // For drop off zone 4
  // Place robot pointing down along left wall in loading zone
  //chasis.Turn(180.f);
  //delay(2000);
  // Get from loading zone to drop off zone
  //delay(1000);
  // LzToDz2();
  // LzToDz2();
  // LzToDz3();
  // LzToDz4();

  // Serial.println(US.F.getDist());
  // Serial.println(IRD.getDist());
  // delay(1000);

  //chasis.Turn(180.f);
  //delay(1000);

  //TurnTowardsBlock(60.f);
  //chasis.Drive(100, Forward);
  //delay(500);
  // chasis.Stop();
  // while (true);

  // Verify orientaton tracking
  chasis.Turn(-90.f);
  Serial.println(chasis.getLook());
  delay(2000);
}
