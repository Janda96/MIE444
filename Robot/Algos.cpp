#include "Algos.h"

// Standard Includes

// Custom Includes
#include "Arduino.h"
#include "Modules.h"
#include "Types.h"
#include "Util.h"

#define WALL_DETECT_DIST 150.f

ErrorCode err = OK;

voidFuncType LzToDz[4] = {&LzToDz1, &LzToDz2, &LzToDz3, &LzToDz4};

// Get to loading zone from random starting location
void GetToLZ()
{
  Serial3.println("Going to loading zone");
  while (!inLoadingZone())
  {
      // GOING LEFT
      if (chasis.getLook() == Left)
      {
          // keep going left until wall hit
          // By following the left wall
          if (isWallDetected(Dir::Left))
          {
            err = chasis.FollowWall(LEFT_WALL);
          }
          else if (isWallDetected(Dir::Right))
          {
            err = chasis.FollowWall(RIGHT_WALL);
          }
          else
          {
            err = chasis.LookFor(LEFT_WALL);
          }

          // if wall hit turn up
          if (err == ObstacleDetected)
          {
            if (inLoadingZone())
            {
              return;
            }
              
            // If up direction blocked go down
            isWallDetected(Dir::Right) ? chasis.Turn(LEFT) : chasis.Turn(RIGHT);
          }

          // if wall disappears look for left wall
          if (err == WallDisapeared)
          {
              chasis.LookFor(LEFT_WALL);
          }
      }

      // GOING UP
      if (chasis.getLook() == Up)
      {
        // look for left wall disappearing
        err = chasis.FollowWall(LEFT_WALL);      // Follow left wall

        // if left wall disappears turn left
        if (err == WallDisapeared)
        {
            chasis.LostWall(LEFT_WALL);
        }

        // if wall hit
        if (err == ObstacleDetected)
        {
            if (inLoadingZone())
            {
              return;
            }

            // If cannot go left, go down. Otherwise go left if clear
            isWallDetected(Dir::Left) ? chasis.Turn(ABOUT_FACE) : chasis.Turn(LEFT);
        }
      }

      // GOING DOWN
      if (chasis.getLook() == Down)
      {
         // Follow right wall
        err = chasis.FollowWall(RIGHT_WALL);    

        // If right wall disapeared, turn right
        err == WallDisapeared ? chasis.LostWall(RIGHT_WALL) : chasis.ClearObstacle();
    }
  }

  // Display confirmation that in loading zone
  lcd.clear();
  lcd.print("IN LOADING ZONE: BLOCK 14");
}

// Travel from Loading zone to top left
// drop off zone
void LzToDz1()
{
  Serial3.println("Going to drop off zone 1");
  
  chasis.FollowWall(LEFT_WALL);       // Follow left wall
  chasis.Turn(RIGHT);                 // Turn right 90 degrees
  chasis.FollowWall(LEFT_WALL);       // Follow Left wall
  chasis.LookFor(RIGHT_WALL);         // Look for right wall
  chasis.FollowWall(RIGHT_WALL);      // Follow right wall
  chasis.Turn(LEFT);                  // Turn left 90 degrees
  chasis.FollowWall(RIGHT_WALL);      // Follow right wall
	chasis.LostWall(LEFT_WALL);         // Lost wall turn left
  chasis.LookFor(RIGHT_WALL);         // Look for right wall
	chasis.FollowWall(RIGHT_WALL);      // Follow right wall
  chasis.Stop();                      // Stop and drop off
}

// Travel from Loading zone to top right
// drop off zone
void LzToDz2()
{
  Serial3.println("Going to drop off zone 2");
    
  chasis.FollowWall(LEFT_WALL);       // Follow left wall
  chasis.Turn(RIGHT);                 // Turn right 90 degrees

  chasis.Stop();
  delay(500);

  chasis.FollowWall(LEFT_WALL);       // Follow Left wall
  chasis.LookFor(RIGHT_WALL);         // Look for right wall
  chasis.FollowWall(RIGHT_WALL);      // Follow right wall
  chasis.Turn(LEFT);                  // Turn left 90 degrees

  chasis.Stop();
  delay(500);

  chasis.FollowWall(RIGHT_WALL);      // Follow right wall

  delay(500);

  chasis.LookFor(LEFT_WALL);          // Look for left wall (drive forward)
  chasis.FollowWall(LEFT_WALL);       // Follow left wall
  chasis.LostWall(LEFT_WALL);         // Lost wall turn left
  chasis.FollowWall(RIGHT_WALL);      // Follow right wall
  chasis.Stop();                      // Stop and drop off
}

// Travel from Loading zone to bottom right
// drop off zone
void LzToDz3()
{
  Serial3.println("Going to drop off zone 3");
    
  chasis.FollowWall(LEFT_WALL);      // Follow left wall
  chasis.Turn(RIGHT);                 // Turn right 90 degrees

  chasis.Stop();
  delay(500);

  chasis.FollowWall(LEFT_WALL);      // Follow Left wall
  chasis.LookFor(RIGHT_WALL);        // Look for right wall
  chasis.FollowWall(RIGHT_WALL);     // Follow right wall
  chasis.Turn(LEFT);                 // Turn left 90 degrees

  chasis.Stop();
  delay(500);

  chasis.FollowWall(RIGHT_WALL);     // Follow right wall

  delay(500);

  chasis.LookFor(RIGHT_WALL);        // Look for Right wall (drive forward)

  chasis.Stop();
  delay(500);

  chasis.FollowWall(RIGHT_WALL);     // Follow Right wall
  chasis.LostWall(RIGHT_WALL);             // Lost wall turn right
  chasis.FollowWall(LEFT_WALL);      // Follow left wall
  chasis.Stop();                      // Stop and drop off
}

// Travel from Loading zone to bottom left
// drop off zone
void LzToDz4()
{
  Serial3.println("Going to drop off zone 4");
    
  chasis.FollowWall(RIGHT_WALL);     // Follow left wall
  chasis.Turn(LEFT);                 // Turn left 90 degrees
  chasis.LookFor(LEFT_WALL);         // Look for left wall
  chasis.FollowWall(LEFT_WALL);      // Look for left wall
  chasis.LostWall(LEFT_WALL);        // Lost wall turn left
  chasis.FollowWall(RIGHT_WALL);     // Follow right wall
  chasis.Stop();
}

bool inLoadingZone()
{
  // Case 1
  if (chasis.getLook() == Up)
  {
    if (isWallDetected(Dir::Front) && isWallDetected(Dir::Left))
    {
      if (!isWallDetected(Dir::Right) && !isWallDetected(Dir::Back))
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
    if (isWallDetected(Dir::Front) && isWallDetected(Dir::Right))
    {
      if (!isWallDetected(Dir::Left) && !isWallDetected(Dir::Back))
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

void DriveIntoBlock(float searchWindowAngle)
{
  TurnTowardsBlock(searchWindowAngle);
  
  chasis.Drive(100.f, Forward);

  // Drive until you get close to block
  while(IRD.getDist() > 55.f)
  {
    // If obstical detected return
    if (isObsticalDetected())
    {
      chasis.Stop();
      return;
    }
  }

  // Drive into the block
  for (auto i = 0; i < 7; ++i)
  {
    if (isObsticalDetected())
    {
      chasis.Stop();
      return;
    }
    delay(100);
  }
  
  chasis.Stop();
}

void TurnTowardsBlock(float searchWindowAngle)
{
  static float angleIncrement = (2.f * searchWindowAngle) / 10.f;

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
  }

  chasis.Turn(minAngle - searchWindowAngle + 30.f);
}

void BlockPickup()
{  
  // Turn servo down to (10) degree to touch the block
  for (int pos = 40; pos >= 10; --pos) 
  {
    MyServo.write(pos);
    delay(20);
  }
  
  // Wait for 1s before raising arm
  delay (1000);
  
  // Raise arm by turning servo up to (130) degree to lift block
  for (int pos = 10; pos <=  130; ++pos) 
  {
    MyServo.write(pos);
    delay(30);
  }
}

void BlockDropoff()
{
    int pos = MyServo.read();
    for (pos; pos <= 180; ++pos) 
    {
      MyServo.write(pos);
      delay(30);
    }
}

void RemoteControl()
{
  int drive;
  int turn;
  while (true)
  {
    if(Serial3.available() > 0)
    {
      if(Serial3.read() == 'd')
      {
        drive = Serial3.parseInt();
        turn = Serial3.parseInt();
        chasis.Drive(drive, Forward);
        
        if(turn == 1)
        {
          chasis.Turn(10);
          Serial3.flush();
        }
      }
    }
  }
}

bool isWallDetected(Dir sensorDir)
{
  return isLessThanDist(sensorDir, WALL_DETECT_DIST);
}

bool isObsticalDetected()
{
  return isLessThanDist(Dir::Front, OBSTACLE_DIST);
}

bool isLessThanDist(Dir sensorDir, float dist)
{
  switch(sensorDir)
  {
    case Dir::Front:
      return US.F.getDist() < dist;
    case Dir::Back:
      return US.B.getDist() < dist;
    case Dir::Right:
      return US.R.getDist() < dist;
    case Dir::Left:
      return US.L.getDist() < dist;
    default:
      return false;
  }
}
