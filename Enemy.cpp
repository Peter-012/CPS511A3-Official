

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

const float PI = 3.14159265;

/*
  :Blimp{x,y,z} is basically calling the superclass Blimp with x,y and z.
  Think of it as super(x,y,z).
*/
Enemy::Enemy(float x, float y, float z, Blimp* player) : Blimp {x,y,z} {
	this->target = player;
	this->lastFireTime = 0;
	this->timeInRange = 0;
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

	bool aiming = false;
	float direction = this->getTargetAngle();
	float next = this->getNextDirection(this->getRotation(), direction);
	
	double targetDiffAngle = abs(direction - next);

	const int ANGLE_DIFFERENCE = 3;
	if (targetDiffAngle > ANGLE_DIFFERENCE) {
		this->setRotation(next);
		aiming = true;
	}
	else {
		this->setRotation(direction);
	}

	//if (currentDist > FIRE_DIST && aiming) {
	//	float distToFire = currentDist - FIRE_DIST;

	//	const float FIRE_DIST_INACCURACY = 1;
	//	if ()
		



	//}
}



//Patrol the area
void Enemy::moveRandomly() {


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



