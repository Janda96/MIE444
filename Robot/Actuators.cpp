#include "Actuators.h"

// Standard Includes

// Custom Includes
#include "Arduino.h"

static const unsigned pwmPins[6] = {3, 5, 6, 9, 10, 11};

Actuator::Actuator(unsigned pinInd, bool isPWM) :
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

void Actuator::set(int val)
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
