#pragma once
// Standard Includes


// Custom Includes

class Sensor
{
public:

  int readValue();

  Sensor(unsigned pinInd, bool isAnalog);
  
private:

private: /* DATA */

  // Pin info
  unsigned pinInd;
  bool isAnalog;
};
