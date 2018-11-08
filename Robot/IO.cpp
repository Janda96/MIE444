#include "IO.h"

// Standard Includes

// Custom Includes
#include "Arduino.h"

static const unsigned pwmPins[] = {1, 5, 6, 7, 15, 16, 17, 18, 23, 24, 25, 26, 38, 39, 40};

Output::Output(unsigned pinInd, bool isPWM) :
pinInd(pinInd),
isPWM(isPWM)
{
  // Set pin and initalize to zero
  pinMode(pinInd, OUTPUT);
  digitalWrite(pinInd, LOW);

  // Make sure valid PWM pin
  if (isPWM)
  {
    isPWM = false;
    for (auto i = 0; i < 6; ++i)
    {
      if (pwmPins[i] == pinInd)
      {
        isPWM = true;
        break;
      }
    }
  }
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
