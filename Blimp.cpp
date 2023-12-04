
#include <gl/glut.h>
#include "header/Blimp.h"
#include <stdio.h>
#include <math.h>
#include "header/Transformations.h"
#include "header/WindowHandler.h"
#include "header/BoundingBox.h"
#include "header/VECTOR3D.h"



GLfloat blimpBlue[] = { 0.0f,0.0f,1.0f,1.0f };
GLfloat blimpYellow[] = { 1.0f,0.7f,0.0f,1.0f};
GLfloat blimpGrey[] = {0.1f, 0.0f, 0.0f, 0.0f};


const float SCALE = 2;
const int RESOLUTION = 48;
const float PI = 3.14159265;



Blimp::Blimp(float locX, float locY, float locZ) {
		this->locX = locX;
		this->locY = locY;
		this->locZ = locZ;
		this->lastY = 0;
		this->lazerScale = 1;

		this->isAlive = true;

		this->velocity = { 0, 0 ,0 }; //basically the velocity of the blimp


		this->rotationHorizontal = 0;
		this->propRotation = 0;
		this->drawLazerToggle = false;

		this->finFanHorizontal = gluNewQuadric();
		this->finFanVertical = gluNewQuadric();
		this->driveLeft = gluNewQuadric();
		this->driveRight = gluNewQuadric();
}


void Blimp::addLocation(float x, float y, float z) {

	this->lastY = y;  //displacement to determine whether it went up/down

	this->locX += x;
	this->locY += y;
	this->locZ += z;


}

void Blimp::cameraPosition(
	float *eyeX, float *eyeY, float *eyeZ,
	float *centerX, float *centerY, float *centerZ
) {
	float angle = this->getRotation();
	float angleRads = (angle * PI) / 180;

	*eyeX = -1 * (this->locZ + (- 1 * sin(angleRads)));
	*eyeY = this->locY;
	*eyeZ = this->locX + cos(angleRads);

	float dist = 5.0;
	*centerX = *eyeX * dist;
	*centerY = *eyeY;
	*centerZ = *eyeZ * dist;
}


void Blimp::moveForward(float x, float z) {
	float angle = this->getRotation();
	float angleRads = (angle * PI) / 180;


	double directionX = cos(angleRads);
	double directionZ = sin(angleRads);

	this->addLocation(directionX * x, 0, -directionZ * z);
}


VECTOR3D Blimp::getDirection() {
	float angle = this->getRotation();
	float angleRads = (angle * PI) / 180;

	double directionX = cos(angleRads);
	double directionZ = sin(angleRads);
	return VECTOR3D(directionX, 0, -directionZ);
}


void Blimp::fireLazer() {
	this->drawLazerToggle = true;
}

void Blimp::stopLazer() {
	this->drawLazerToggle = false;
}


/*             90
               |
               |
      180----------------0
	           |
			   |
			  270

*/
void Blimp::setRotation(float angDeg) {

	if (angDeg >= 360)
		angDeg -= 360;

	if (angDeg <= 0) {
		angDeg += 360;
	}

	this->rotationHorizontal = angDeg;
}


void Blimp::setLocation(float x, float y, float z) {
	this->locX = x;
	this->locY = y;
	this->locZ = z;
}

void Blimp::setLazerScale(float scale) {
	this->lazerScale = scale;
}


float Blimp::getRotation() {
	return this->rotationHorizontal;
}


bool Blimp::getIsAlive() {
	return this->isAlive;
}


float Blimp::getX() {
	return this->locX;
}

float Blimp::getY() {
	return this->locY;
}

float Blimp::getZ() {
	return this->locZ;
}

void Blimp::setLazerScaleUpdated(bool updated) {
	this->lazerScaleUpdated = updated;
}

float Blimp::getBottomBlimpY() {
	float bodyHeight = SCALE / 2;
	float cabinHeight = SCALE / 4;
	return (bodyHeight / 2) - cabinHeight - SCALE;
}


BoundingBox Blimp::getCurrentBoundingBoxState() {
	float minX, maxX, minY, maxY, minZ, maxZ;
	const float BOX_SCALE = 2;

	minX = this->getX() - BOX_SCALE;
	maxX = this->getX() + BOX_SCALE;

	minY = this->getY() - BOX_SCALE;
	maxY = this->getY() + BOX_SCALE;

	minZ = this->getZ() - BOX_SCALE;
	maxZ = this->getZ() + BOX_SCALE;

	return BoundingBox(minX, maxX, minY, maxY, minZ, maxZ);

}







//============================================================================================================
//Drawing functions below here.


void Blimp::draw() {

		glPushMatrix();  //the matrix stack is still there after you exit the function. If you do this and don't pop
		                     //it'll break
		
     	translateCustom(locX, locY, locZ);
		//glTranslatef(locX, locY, locZ);

		rotateCustom(rotationHorizontal, 0, 1, 0);
		//glRotatef(rotationHorizontal, 0, 1, 0);

		glPushMatrix();
		drawBody();
		drawCarriage();
		drawFins();
		drawEngine();

		if (this->drawLazerToggle) {
			drawLazer();
		}
		
		propRotation += 10;
		if (propRotation > 360) {
			propRotation -= 360;
		}

		glPopMatrix();
}



void Blimp::drawBody() {
	glPushMatrix();



	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, blimpBlue);
	glMatrixMode(GL_MODELVIEW);

	//glScalef(SCALE, SCALE / 2, SCALE / 2);
	scaleCustom(SCALE, SCALE / 2, SCALE / 2);

	glutSolidSphere(SCALE, RESOLUTION, RESOLUTION);
	glPopMatrix();
}




void Blimp::drawCarriage() {

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, blimpYellow);


	glPushMatrix();
	//glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, blimpYellow);
	
	translateCustom(0, -SCALE, 0);
	//glTranslatef(0, -SCALE, 0);
	
	scaleCustom(SCALE / 3, SCALE / 4, SCALE / 6);
	//glScalef(SCALE/3, SCALE/4, SCALE/6);

	glutSolidCube(SCALE);
	glPopMatrix();

}



void Blimp::drawLazer() {



	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, blimpYellow);
	glPushMatrix();

		const float DEFAULT_DIST_SCALE = 8 * SCALE;

	float distanceLazerScale;
	if (this->lazerScaleUpdated) {
		distanceLazerScale = this->lazerScale * 0.25;
	}
	else {
		distanceLazerScale = DEFAULT_DIST_SCALE;
	}

	translateCustom(distanceLazerScale * 2, -1 * (SCALE + (SCALE / 10)), 0);

	scaleCustom(distanceLazerScale, SCALE / 20, SCALE / 20);
	glutSolidSphere(SCALE, RESOLUTION, RESOLUTION);
	glPopMatrix();
}




void Blimp::drawFins() {

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, blimpBlue);
	glPushMatrix();

	translateCustom(-SCALE * 2, 0, 0);
	//glTranslatef(-SCALE * 2, 0, 0);

	glPushMatrix();
	
    //glRotatef(90, 1, 0, 0);
	rotateCustom(90, 1, 0, 0);


	gluPartialDisk(this->finFanHorizontal, 0, SCALE, 10, 3, 0, 180);
	glPopMatrix();
	gluPartialDisk(this->finFanVertical, 0, SCALE, 10, 3, 0, 180);


	glPopMatrix();
	glPopMatrix();

}


void Blimp::drawEngine() {

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, blimpYellow);
	float engineScale = SCALE / 3;



	float offset = 0;

	if (lastY > 0) {
		offset = 90;
	}
	else if (lastY < 0) {
		offset = -90;
	}


	//right engine
	glPushMatrix();
	
	//glTranslatef(0, 0, SCALE + engineScale);
	translateCustom(0, 0, SCALE + engineScale);

	glPushMatrix();

	//glRotatef(90, 0, 1, 0);   //positioning of the engine
	rotateCustom(90, 0, 1, 0);

	//glRotatef(-offset, 1, 0, 0);  //tilting the things up or down
	rotateCustom(-offset, 1, 0, 0);

	glutSolidCone(engineScale, engineScale * 1.5, 24, 24);   //engine body
	glPopMatrix();


	   glPushMatrix();
	//glRotatef(90, 0, 1, 0);
	   rotateCustom(90, 0, 1, 0);
	 
	
	//glRotatef(180 - offset, 1, 0, 0);
	   rotateCustom(180 - offset, 1, 0, 0);

	gluCylinder(driveRight, engineScale/4, engineScale/4, engineScale, 12, 12);  //driveshaft
	   glPopMatrix();


	  glPushMatrix(); //prop blades

	//  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, blimpGrey);
	  //glRotatef(90, 0, 1, 0);
	  rotateCustom(90, 0, 1, 0);
	  //glRotatef(180 - offset, 1, 0, 0);
	  rotateCustom(180 - offset, 1, 0, 0);
	  //glTranslatef(0, 0, engineScale);
	  translateCustom(0, 0, engineScale);
	  //glRotatef(propRotation, 0, 0, 1);
	  rotateCustom(propRotation, 0, 0, 1);
	  //glScalef(engineScale * 2, engineScale / 4, engineScale / 8);
	  scaleCustom(engineScale * 2, engineScale / 4, engineScale / 8);
	
	glutSolidCube(1);

	   glPopMatrix();  //prop blades
	   glPopMatrix();




	//   glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, blimpYellow);

	//left engine
	glPushMatrix();
	//glTranslatef(0, 0, -(SCALE + engineScale));
	translateCustom(0, 0, -(SCALE + engineScale));
	glPushMatrix(); 

	//glRotatef(90, 0, 1, 0);   //positioning of the engine
	rotateCustom(90, 0, 1, 0);
	
	//glRotatef(-offset, 1, 0, 0);  //tilting the things up or down
	rotateCustom(-offset, 1, 0, 0);

	glutSolidCone(engineScale, engineScale * 1.5, 24, 24);   //engine body
	glPopMatrix();


	glPushMatrix();
	//glRotatef(90, 0, 1, 0);
	rotateCustom(90, 0, 1, 0);
	
	//glRotatef(180 - offset, 1, 0, 0);
	rotateCustom(180 - offset, 1, 0, 0);

	gluCylinder(driveRight, engineScale / 4, engineScale / 4, engineScale, 12, 12);  //driveshaft
	glPopMatrix();


	glPushMatrix(); //prop blades

	//glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, blimpGrey);
	//glRotatef(90, 0, 1, 0);
	rotateCustom(90, 0, 1, 0);
	
	//glRotatef(180 - offset, 1, 0, 0);
	rotateCustom(180 - offset, 1, 0, 0);
	//glTranslatef(0, 0, engineScale);
	translateCustom(0, 0, engineScale);
	
	//glRotatef(-propRotation, 0, 0, 1);
	rotateCustom(-propRotation, 0, 0, 1);
	//glScalef(engineScale * 2, engineScale / 4, engineScale / 8);
	scaleCustom(engineScale * 2, engineScale / 4, engineScale / 8);
	glutSolidCube(1);

	glPopMatrix();  //prop blades
	glPopMatrix();
	
 
}
