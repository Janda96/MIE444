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
	// Follow left wall
	// Turn right 90 degrees
	// Follow Left wall
	// Look for right wall
	// Turn left 90 degrees
	// Follow right wall
	// DealWithLostWall Turn left
	// Follow right wall
	// Stop and drop off
}

// Travel from Loading zone to top right
// drop off zone
void LzToDz2()
{
	// Follow left wall
	// Turn right 90 degrees
	// Follow Left wall
	// Look for right wall
	// Turn left 90 degrees
	// Follow right wall
	// Look for left wall (drive forward)
	// Follow left wall
	// DealWithLostWall Turn left
	// Follow right wall
	// Stop and drop off
}

// Travel from Loading zone to bottom right
// drop off zone
void LzToDz3()
{
	// Follow left wall
	// Turn right 90 degrees
	// Follow Left wall
	// Look for right wall
	// Turn left 90 degrees
	// Follow right wall
	// Look for left wall (drive forward)
	// Follow left wall
	// DealWithLostWall Turn Right
	// Follow left wall
	// Stop and drop off
}

// Travel from Loading zone to bottom left
// drop off zone
void LzToDz4()
{
	// Follow right wall
	// Turn left 90 degrees
	// Find left wall
	// DealWithLostWall Turn left
	// Follow right wall
	// Stop and drop off
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
