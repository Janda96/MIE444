#include "Algos.h"

// Standard Includes

// Custom Includes
#include "Arduino.h"
#include "Modules.h"
#include "Types.h"

ErrorCode err = OK;

// Get to loading zone from random starting location
void GetToLZ()
{
  Pose p;
  float var;
  while (!isLoadingZone())
  {
      // Find orientation
      Locator.getPose(p, var);
      if (p.O == Left)
      {
          // keep going left until wall hit
          // By following the left wall
          err = chasis.FollowWall(US.L, true);

          // if wall hit turn up
          if (err == ObstacleDetected)
          {
              chasis.Turn(-90);
          }

          // if wall disappears look for left wall
          if (err == WallDisapeared)
          {
              chasis.LookFor(US.L);  
          }
      }
      if (p.O == Up)
      {
        // look for left wall disappearing
        err = chasis.FollowWall(US.L, true);      // Follow left wall

        // if left wall disappears turn left
        if (err == WallDisapeared)
        {
            chasis.LostWall(true);
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

bool isLoadingZone()
{
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
