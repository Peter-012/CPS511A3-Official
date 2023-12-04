#include <gl/glut.h>
#include "header/Missile.h"
#include <stdio.h>
#include <math.h>
#include "header/Transformations.h"
#include "header/Util.h"

GLfloat missileBlue[] = { 0.0f,0.0f,1.0f,1.0f };

const float SCALE = 1;
const int RESOLUTION = 48;

Missile::Missile(float locX, float locY, float locZ, float rotationY) {
	this->locX = locX;
	this->locY = locY;
	this->locZ = locZ;
	this->rotationY = rotationY;
}


void Missile::addLocation(float x, float y, float z) {
	this->locX += x;
	this->locY += y;
	this->locZ += z;
}

void Missile::draw() {
	translateCustom(locX, locY, locZ);
	

	glPushMatrix();
	drawBody();
	glPopMatrix();

	const float velocity = 0.1;

	locX += velocity * cos(rotationY);
	locZ += velocity * sin(rotationY);
}

void Missile::drawBody() {
	glPushMatrix();

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, missileBlue);
	glMatrixMode(GL_MODELVIEW);

	rotateCustom(toDegrees(-1 * rotationY), 0, 1.0, 0);
	scaleCustom(SCALE, SCALE / 3, SCALE / 3);

	glutSolidSphere(SCALE, RESOLUTION, RESOLUTION);
	glPopMatrix();
}
