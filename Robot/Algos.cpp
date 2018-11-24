#include "Algos.h"

// Standard Includes

// Custom Includes
#include "Arduino.h"
#include "Modules.h"
#include "Types.h"
#include "Util.h"

ErrorCode err = OK;

// Get to loading zone from random starting location
void GetToLZ()
{
  while (!inLoadingZone())
  {
      // Find orientation
      if (chasis.getLook() == Left)
      {    
          // keep going left until wall hit
          // By following the left wall
          err = chasis.FollowWall(US.L, true);

          // if wall hit turn up
          if (err == ObstacleDetected)
          {
            if (inLoadingZone())
            {
              return;
            }
            chasis.Turn(-90.f);
          }

          // if wall disappears look for left wall
          if (err == WallDisapeared)
          {
              chasis.LookFor(US.L);  
          }
      }
      if (chasis.getLook() == Up)
      { 
        chasis.Drive(100.f, Forward);
        
        // look for left wall disappearing
        err = chasis.FollowWall(US.L, true);      // Follow left wall

        // if left wall disappears turn left
        if (err == WallDisapeared)
        {
            chasis.LostWall(true);
        }

        // if wall hit
        if (err == ObstacleDetected)
        {
            if (inLoadingZone())
            {
              return;
            }
            
            // If cannot go left, go down
            if (US.L.getDist() < 100.f)
            {
              chasis.Turn(180.f);
            }
            // Go left if clear
            else
            {
              chasis.Turn(90.f);
            }
        }
      }
      if (chasis.getLook() == Down)
      {
        // look for left wall disappearing
        err = chasis.FollowWall(US.R, true);      // Follow left wall
        if (err == WallDisapeared)
        {
          chasis.LostWall(false);
        }
        else
        {
          chasis.ClearObstical();
        }
    }
  }
}

// Travel from Loading zone to top left
// drop off zone
void LzToDz1()
{
  chasis.FollowWall(US.L, true);      // Follow left wall
  chasis.Turn(-90);                   // Turn right 90 degrees
  chasis.FollowWall(US.L, true);      // Follow Left wall
  chasis.LookFor(US.R);               // Look for right wall
  chasis.FollowWall(US.R, false);     // Follow right wall
  chasis.Turn(90);                    // Turn left 90 degrees
  chasis.FollowWall(US.R, false);     // Follow right wall
	chasis.LostWall(true);              // Lost wall turn left
  chasis.LookFor(US.R);               // Look for right wall
	chasis.FollowWall(US.R, false);     // Follow right wall
  chasis.Stop();                      // Stop and drop off
}

// Travel from Loading zone to top right
// drop off zone
void LzToDz2()
{
  chasis.FollowWall(US.L, true);      // Follow left wall
  chasis.Turn(-90);                   // Turn right 90 degrees
  
  chasis.Stop();
  delay(500);
  
  chasis.FollowWall(US.L, true);      // Follow Left wall
  chasis.LookFor(US.R);               // Look for right wall
  chasis.FollowWall(US.R, false);     // Follow right wall
  chasis.Turn(90);                    // Turn left 90 degrees
  
  chasis.Stop();
  delay(500);
  
  chasis.FollowWall(US.R, false);     // Follow right wall
  
  chasis.Stop();
  delay(500);
  
  chasis.LookFor(US.L);               // Look for left wall (drive forward)
  chasis.FollowWall(US.L, true);      // Follow left wall
  chasis.LostWall(true);              // Lost wall turn left
  chasis.FollowWall(US.R, false);     // Follow right wall
  chasis.Stop();                      // Stop and drop off
}

// Travel from Loading zone to bottom right
// drop off zone
void LzToDz3()
{
  chasis.FollowWall(US.L, true);      // Follow left wall
  chasis.Turn(-90);                   // Turn right 90 degrees
  chasis.FollowWall(US.L, true);      // Follow Left wall
  chasis.LookFor(US.R);               // Look for right wall
  chasis.Turn(90);                    // Turn left 90 degrees
  chasis.FollowWall(US.R, false);     // Follow right wall
  chasis.LookFor(US.L);               // Look for left wall (drive forward)
  chasis.FollowWall(US.L, true);      // Follow left wall
  chasis.LostWall(false);             // Lost wall turn right
  chasis.FollowWall(US.L, true);      // Follow left wall
  chasis.Stop();                      // Stop and drop off
}

// Travel from Loading zone to bottom left
// drop off zone
void LzToDz4()
{
  chasis.FollowWall(US.R, false);     // Follow left wall
  chasis.Turn(90);                    // Turn left 90 degrees
  chasis.LookFor(US.L);               // Look for left wall
  chasis.LostWall(true);              // Lost wall turn left
  chasis.FollowWall(US.R, false);     // Follow right wall
  chasis.Stop();
}

bool inLoadingZone()
{
  // Case 1
  if (chasis.getLook() == Up)
  {
    if (US.F.getDist() < 100.f && US.L.getDist() < 100.f)
    {
      if (US.R.getDist() > 100.f && US.B.getDist() > 100.f)
      {
        return true;
      }
      return false;
    }
    else
    {
      return false;
    }
  }
  // Case 2
  else if (chasis.getLook() == Left)
  {
    if (US.F.getDist() < 100.f && US.R.getDist() < 100.f)
    {
      if (US.L.getDist() > 100.f && US.B.getDist() > 100.f)
      {
        return true;
      }
      return false;
    }
    else
    {
      return false;
    }
  }

  // Default Not in loading zone
  return false;
}

bool isBlockDetected()
{
	float topDist = US.F.getDist();
	float bottomDist = IRD.getDist();
	
	return topDist > 200.f && bottomDist < 200.f;
}

void TurnTowardsBlock(float searchWindowAngle)
{
  static float angleIncrement = (2.f * searchWindowAngle) / 20.f;
  
  chasis.Turn(-1.f * searchWindowAngle);
 
  // Sweep to find block
  float minDist = 500.f;
  float minAngle = 0.f;
  float dist = 500.f;
  for (auto angle = -1.f * searchWindowAngle; angle < searchWindowAngle + 0.1f; angle += angleIncrement)
  {
    dist = IRD.getDist();
    if (dist < minDist)
    {
        minAngle = angle;
        minDist = dist;
    }
    chasis.Turn(angleIncrement);
    delay(500);
    Serial.println(angle);
    Serial.println(dist);
  }
  

  chasis.Turn(minAngle - searchWindowAngle + 50);
  Serial.println();
  Serial.println(minAngle);
}

// Localization Code
#define WALL_DIST 100.f
#define NUM_MEASUREMENTS 20
char orientationArr[NUM_MEASUREMENTS];
char USReadingArr[NUM_MEASUREMENTS];

int sumIntArr(int* arr, int sizeOfArr)
{
  int sum = 0;
  for (auto i = 0; i < sizeOfArr; ++i)
  {
    sum += arr[i];
  }
  return sum;
}

bool areWallsOpposite(int* arr)
{
  if (arr[0] == 1 && arr[1] == 1)
  {
    return true;
  }
  else if (arr[2] == 1 && arr[3] == 1)
  {
    return true;
  }
  else
  {
    return false;
  }
}

char getMappedUSReadings()
{
  bool isFrontWall = US.F.getDist() < WALL_DIST;
  bool isBackWall = US.B.getDist() < WALL_DIST;
  bool isRightWall = US.R.getDist() < WALL_DIST;
  bool isLeftWall = US.L.getDist() < WALL_DIST;

  int walls[4] = {isFrontWall, isBackWall, isRightWall, isLeftWall};

  if (sumIntArr(walls, 4) == 0)
  {
      return '0';
  }
  if (sumIntArr(walls, 4) == 1)
  {
      return '1';
  }
  if (sumIntArr(walls, 4) == 3)
  {
      return '3';
  }
  if (sumIntArr(walls, 4) == 2)
  {
    if (areWallsOpposite(walls))
    {
      return '5';
    }
    else
    {
      return '2';
    }
  }
  else
  {
    return '4';
  }
}

void TakeLocMeasurement(int ind)
{
  USReadingArr[ind] = getMappedUSReadings();

  char look;
  Orientation O = chasis.getLook();
  if (O == Up)
  {
    look = 'U';
  }
  if (O == Down)
  {
    look = 'D';
  }
  if (O == Left)
  {
    look = 'L';
  }
  if (O == Right)
  {
    look = 'R';
  }
  orientationArr[ind] = look;
}

void SendLocMeasurements()
{
  for (auto i = 0; i < NUM_MEASUREMENTS; ++i)
  {
    Serial3.println(orientationArr[i]);
    Serial3.println(USReadingArr[i]);
  }
}

void Localize()
{
  for (auto i = 0; i < NUM_MEASUREMENTS; ++i)
  {
    TakeLocMeasurement(i);
    chasis.Drive(100.f, Forward);
    delay(500);
    chasis.Stop();
  }
  SendLocMeasurements();
}
