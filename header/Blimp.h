#pragma once

#include <gl/glut.h>;
#include <vector>;

#ifndef BLIMP_H
#define BLIMP_H


class Blimp {


public:
	Blimp(float locX, float locY, float locZ);
	
	void setRotation(float newRotation);
	float getRotation();
	void draw();
	void setLocation(float x, float y, float z);
	void addLocation(float x, float y, float z);
	float getX();
	float getY();
	float getZ();
	void fireMissile();


	std::vector<float> getLocation();
	void moveForward(float x, float z);
	bool getIsAlive();
	



	


protected:
	float locX;
	float locY;
	float locZ;

	float lastY;

	float propRotation;


	bool isAlive;
	
	std::vector<float> velocity;

	float rotationHorizontal;
	GLUquadricObj *finFanHorizontal, *finFanVertical, *driveLeft, *driveRight;

	

	void drawBody();
	void drawFins();
	void drawCarriage();
	void drawEngine();

};

#endif