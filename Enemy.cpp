

#include "header/Enemy.h"
#include "header/Blimp.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "header/VECTOR3D.h"
#include "header/Util.h"
#include <algorithm>

//dist where the enemy should start tracking the player.
const float TRACKING_DIST = 32;
const float FIRE_DIST = 16;
const float FIRE_NEAR = 5;
const long long FIRE_COOLDOWN_ENEMY = 1000;
const long long PATROL_TIME = 1000;

const float PI = 3.14159265;

/*
  :Blimp{x,y,z} is basically calling the superclass Blimp with x,y and z.
  Think of it as super(x,y,z).
*/
Enemy::Enemy(float x, float y, float z, Blimp* player) : Blimp {x,y,z} {
	this->target = player;
	this->lastFireTime = 0;
	this->lastPatrolAngleSwitchTime = 0;
	this->idleTargetAngle = 0;

}

void Enemy::tick() {
	if (!target->getIsAlive()) {
		return;
	}

	float diffX = (target->getX() - this->getX());
	float diffY = (target->getY() - this->getY());
	float diffZ = (target->getZ() - this->getZ());

	float distFromTarget = sqrt(abs(diffX*diffX  + diffY*diffY  +  diffZ*diffZ));

	if (distFromTarget > TRACKING_DIST) {
		this->moveRandomly();
		return;
	}

	this->trackTarget(distFromTarget);
}



// Move towards the target and try to fire a missile
void Enemy::trackTarget(float currentDist) {


	const int ANGLE_MAX = 90;

	//the threshold where we set the vertical location instead of adding to it
	const float VERTICAL_INCREMENT = 0.05;

	const float DIST_INCREMENT = 0.1;


	///logic for aiming at the player
	
	float direction = this->getTargetAngle();
	bool aiming = turnTowardsAngle(direction);

	
	///logic for going up and down
	float diffY = (target->getY() - this->getY());

	if (abs(diffY) < VERTICAL_INCREMENT) {
		this->setLocation(this->getX(), target->getY(), this->getZ());
	}
	else {
		if (diffY > 0) {
			this->addLocation(0, VERTICAL_INCREMENT, 0);
		}
		else if (diffY < 0) {
			this->addLocation(0, -VERTICAL_INCREMENT, 0);
		}
	}
	

	bool inRange = false;
	if (currentDist > FIRE_DIST && aiming) {
		float distToFire = abs(currentDist - FIRE_DIST);

		if (!aiming) {
			return;
		}
		
		if (distToFire >= FIRE_NEAR + 1) {
			if (distToFire < DIST_INCREMENT) {
				inRange = true;
			}
			else {
				this->moveForward(DIST_INCREMENT, DIST_INCREMENT);
			}
		}
		else {

			if (distToFire < DIST_INCREMENT) {
				inRange = true;
			}
			else {
				if (distToFire < FIRE_NEAR - 1) {
					this->moveForward(-DIST_INCREMENT, -DIST_INCREMENT);
				}
			}
		}	
	}


	long long currentTime = currentTimeMillis();
	if (currentTime - this->lastFireTime > FIRE_COOLDOWN_ENEMY) {
			this->lastFireTime = currentTime;

			printf("Enemy fires missiles here");
		
	}

}




//Patrol the area
void Enemy::moveRandomly() {

	bool changed = false;
	long long currentTime = currentTimeMillis();
	//printf("%d\n", currentTime);

	long long diff = currentTime - this->lastPatrolAngleSwitchTime;
	
	if (diff > PATROL_TIME) {
		changed = true;
		idleTargetAngle = rand() % 360;
		this->lastPatrolAngleSwitchTime = currentTimeMillis();
	}
	
	this->moveForward(0.1, 0.1);
	this->turnTowardsAngle(idleTargetAngle);
	
	


}








//curAngle, targetAngle are in degrees
float Enemy::getNextDirection(float currentAngle, float targetAngle) {


	double diff = targetAngle - currentAngle + 540;
	double diffAngle = ((int)(diff)) % 360 - 180;


	double direction;
	if (diffAngle > 0) {
		direction = 1;
	}
	else if (diffAngle < 0) {
		direction = -1;
	}
	else direction = 0;


	double offset;
	double absDiff = abs(diffAngle);

	if (absDiff >= 1) {
		offset = 1;
	}
	else {
		offset = absDiff;
	}

	direction *= offset;
	currentAngle += (direction * 0.5);
	return currentAngle;
}


float Enemy::getTargetAngle() {
	//work out the angle we need to face to fire the thing
	float diffX = (target->getX() - this->getX());
	float diffZ = (target->getZ() - this->getZ());


	float horAngle = 0;

	double horDist = sqrt(diffX * diffX + diffZ * diffZ);


	if (diffZ == 0 && diffX == 0) {
		return this->getRotation();
	}


	if (diffZ == 0 || diffX == 0) {
		if (diffZ == 0) {
			horAngle = diffX > 0 ? 0 : toRadians(180);
		}

		if (diffX == 0) {
			horAngle = diffZ > 0 ? toRadians(90) : toRadians(270);
		}
	}
	else {
		horAngle = atan(diffZ / diffX);

		if (diffX < 0) {
			horAngle += PI;
		}
	}

    return toDegrees(-horAngle);
}


bool Enemy::turnTowardsAngle(float direction) {

	//the threshold where we set the rotation instead of adding to the rotation
	const int ANGLE_DIFFERENCE = 3;
	float next = this->getNextDirection(this->getRotation(), direction);
	double targetDiffAngle = abs(direction - next);

	if (targetDiffAngle > ANGLE_DIFFERENCE) {
		this->setRotation(next);
		return true;
	}
	else {
		this->setRotation(direction);
		return false;
	}

}

