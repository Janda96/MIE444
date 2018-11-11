#include "IO.h"

// Standard Includes

// Custom Includes
#include "Arduino.h"

Output::Output(unsigned pinInd, bool isPWM) :
pinInd(pinInd),
isPWM(isPWM)
{
  // Set pin and initalize to zero
  pinMode(pinInd, OUTPUT);
  digitalWrite(pinInd, LOW);
}

void Output::set(int val)
{
  if (isPWM)
  {
    analogWrite(pinInd, val);
  }
  else
  {
    digitalWrite(pinInd, static_cast<bool>(val));
  }
}

int Input::readValue()
{
  return isAnalog ? analogRead(pinInd) : digitalRead(pinInd);
}

unsigned long Input::timePulse(int value)
{
  return pulseIn(pinInd, value);
}

Input::Input(unsigned pinInd, bool isAnalog) :
pinInd(pinInd),
isAnalog(isAnalog)
{
  pinMode(pinInd, INPUT);
}
