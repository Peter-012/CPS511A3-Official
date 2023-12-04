#pragma once

#include <gl/glut.h>;
#include <vector>;
#include "header/BoundingBox.h"
#include "VECTOR3D.h"

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
	float getBottomBlimpY();
	void fireLazer(); ////
	void stopLazer();
	BoundingBox getCurrentBoundingBoxState();
	VECTOR3D getDirection();

	void moveForward(float x, float z);
	void cameraPosition(
		float* eyeX, float* eyeY, float* eyeZ,
		float* centerX, float* centerY, float* centerZ
	);
	bool getIsAlive();


	void setLazerScale(float scale);
	void setLazerScaleUpdated(bool updated);
	
	
protected:
	float locX;
	float locY;
	float locZ;

	float lastY;

	float propRotation;

	bool drawLazerToggle;
	float lazerScale;
	bool lazerScaleUpdated;

	bool isAlive;
	
	std::vector<float> velocity;

	float rotationHorizontal;
	GLUquadricObj *finFanHorizontal, *finFanVertical, *driveLeft, *driveRight;

	

	void drawBody();
	void drawFins();
	void drawCarriage();
	void drawEngine();
	void drawLazer();

};

#endif