#pragma once

#include <gl/glut.h>;
#include <vector>;

class Missile {
public:
	Missile(float locX, float locY, float locZ, float rotationY);

	void draw();
	void addLocation(float x, float y, float z);

private:
	float locX;
	float locY;
	float locZ;

	float rotationY;

	void drawBody();
};