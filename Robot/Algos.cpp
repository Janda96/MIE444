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
      if (p.o == Left)
      {
          // keep going left until wall hit
          // By following the left wall
          err = chasis.FollowWall(US.L);

          // if wall hit turn up
          if (err == ObstacleDetected)
          {
              chasis.turn(-90)    
          }

          // if wall disappears look for left wall
          if (err == WallDisapeared)
          {
              chasis.LookFor(true);  
          }
      }
      if (p.o == Up)
      {
        // look for left wall disappearing
        err = chasis.FollowWall(US.L);      // Follow left wall

        // if left wall disappears turn left
        if (err == WallDisapeared)
        {
            chasis.WallLost(true);
        }
      }
    }
  }
}

// Travel from Loading zone to top left
// drop off zone
void LzToDz1()
{
  chasis.FollowWall(US.L);      // Follow left wall
  chasis.Turn(-90);             // Turn right 90 degrees
  chasis.FollowWall(US.L);      // Follow Left wall
  chasis.LookFor(US.R);         // Look for right wall
  chasis.Turn(90);              // Turn left 90 degrees
  chasis.FollowWall(US.R);      // Follow right wall
	chasis.LostWall(true);        // Lost wall turn left
	chasis.FollowWall(US.R);      // Follow right wall
  chasis.Stop();                // Stop and drop off
}

// Travel from Loading zone to top right
// drop off zone
void LzToDz2()
{
  chasis.FollowWall(US.L);      // Follow left wall
  chasis.Turn(-90);             // Turn right 90 degrees
  chasis.FollowWall(US.L);      // Follow Left wall
  chasis.LookFor(US.R);         // Look for right wall
  chasis.Turn(90);              // Turn left 90 degrees
  chasis.FollowWall(US.R);      // Follow right wall
  chasis.LookFor(true);         // Look for left wall (drive forward)
  chasis.FollowWall(US.L);      // Follow left wall
  chasis.LostWall(true);        // Lost wall turn left
  chasis.FollowWall(US.R);      // Follow right wall
  chasis.Stop();                // Stop and drop off
}

// Travel from Loading zone to bottom right
// drop off zone
void LzToDz3()
{
  chasis.FollowWall(US.L);      // Follow left wall
  chasis.Turn(-90);             // Turn right 90 degrees
  chasis.FollowWall(US.L);      // Follow Left wall
  chasis.LookFor(US.R);         // Look for right wall
  chasis.Turn(90);              // Turn left 90 degrees
  chasis.FollowWall(US.R);      // Follow right wall
  chasis.LookFor(true);         // Look for left wall (drive forward)
  chasis.FollowWall(US.L);      // Follow left wall
  chasis.LostWall(false);       // Lost wall turn right
  chasis.FollowWall(US.L);      // Follow left wall
  chasis.Stop();                // Stop and drop off
}

// Travel from Loading zone to bottom left
// drop off zone
void LzToDz4()
{
  chasis.FollowWall(US.R);      // Follow left wall
  chasis.Turn(90);              // Turn left 90 degrees
  chasis.LookFor(US.L);         // Look for left wall
  chasis.LostWall(true);        // Lost wall turn left
  chasis.FollowWall(US.R);      // Follow right wall
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
	static float angleIncrement = 5.f;
	
	// Sweep to find block
	float leftEdgeAngle = 0.f;
	float rightEdgeAngle = 0.f;
	bool foundLeft = false;
	for (auto angle = -searchWindowAngle; angle < searchWindowAngle; angle += angleIncrement)
	{
		if (isBlockDetected)
		{
			if (foundLeft)
			{
				rightEdgeAngle = angle;
			}
			else
			{
				leftEdgeAngle = angle;
				foundLeft = true;
			}
		}
	}
	
	float centerAngle = (rightEdgeAngle - leftEdgeAngle) / 2.f;
	if (centerAngle > 0.f)
	{
		chasis.Turn(centerAngle - 2 * searchWindowAngle);
	}
}
