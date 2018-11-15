#include "Algos.h"

// Standard Includes

// Custom Includes
#include "Arduino.h"
#include "Modules.h"

// Get to loading zone from random starting location
void GetToLZ()
{
	// while not in the loading zone
		// Find orientation
		// if going left
			// keep going left until wall hit
			// if wall hit
				// turn up
			// if wall disappears
				// look for left wall
		// if going up
			// look for left wall disappearing
			// if left wall disappears
				// DealWithLostWall(isLeft = true)
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
