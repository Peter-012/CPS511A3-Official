#pragma once
#ifndef ENEMY_H
#define ENEMY_H

#include "Blimp.h"
#include "Time.h"



class Enemy: public Blimp {

public:
	
	Enemy(float locX, float locY, float locZ, Blimp* player);
	void tick();
	void moveRandomly();
	void trackTarget(float currentDist);
	float getTargetAngle();
	float getNextDirection(float curAngle, float nextAngle);

	
	


private:
	Blimp* target;
	long long lastFireTime;
	long timeInRange;



};





#endif