
/*
 CPS511 A3

 @author: 
 Talon Jiang - 501125811
 Peter Lee - 501088502
*/


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <gl/glut.h>
#include <utility>
#include <vector>
#include "header/VECTOR3D.h"
#include "header/QuadMesh.h"
#include "header/Blimp.h"
#include "header/Transformations.h"
#include <chrono>
#include "header/GroundMesh.h"
#include "header/WindowHandler.h"
#include "header/Enemy.h"
#include "header/Missile.h"
#include "header/Util.h"


const int VIEWPORT_WIDTH  = 800;    // Viewport width in pixels
const int VIEWPORT_HEIGHT = 600;    // Viewport height in pixels

int windowBird, windowFPV;

GroundMesh* ground = new GroundMesh(16, 0, -20, 5);
Blimp* blimp = new Blimp(0, -1, 15, 90.0);
Enemy* blimp2 = new Enemy(0, -1, -10, -90.0, blimp);
Missile* missile1;

bool firePlayerLazer = false;

int idleTime = 0;

const float PI = 3.14159265;


// Default Mesh Size
int meshSize = 16;

// Prototypes for functions in this module

void displayIdleBird(void);
void displayIdleFPV(void);
void keyboard(unsigned char key, int x, int y);
void keyboardSpecial(int key, int x, int y);
void displayBird();
void displayFPV();



int main(int argc, char **argv)
{
	// Initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(VIEWPORT_WIDTH, VIEWPORT_HEIGHT);
	glutInitWindowPosition(950, 200);
	
	// Bird's Eye View Window
	windowBird = glutCreateWindow("Blimp - Bird's Eye View");
	initOpenGL(VIEWPORT_WIDTH, VIEWPORT_HEIGHT);
	glutDisplayFunc(displayBird);
	glutReshapeFunc(reshape);
	glutSpecialFunc(keyboardSpecial);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(displayIdleBird);
	
	// First Person View Window
	windowFPV = glutCreateWindow("Blimp - First Person View");
	glutPositionWindow(150, 200);
	initOpenGL(VIEWPORT_WIDTH, VIEWPORT_HEIGHT);
	glutDisplayFunc(displayFPV);
	glutReshapeFunc(reshape);
	glutSpecialFunc(keyboardSpecial);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(displayIdleFPV);
	
	glutMainLoop();

	return 0;
}


void keyboard(unsigned char key, int x, int y) {

	idleTime = 0;
	switch (key) {
	case 'w': {
		blimp->moveForward(0.5, 0.5);
		//blimp2->moveForward(0.2, 0.2);
	  }
	break;
	case 's': {
		blimp->moveBackward(0.5, 0.5);
		//blimp2->moveForward(0.2, 0.2);
	}
	break;

	case ' ': {
		if (!firePlayerLazer) {
			blimp->fireLaser();
			firePlayerLazer = true;
		} else {
			blimp->stopLaser();
			firePlayerLazer = false;
		}	
	}
	break;
	}

	// Trigger a window redisplay
	glutPostRedisplay();
	glutSetWindow(windowBird);
	glutPostRedisplay();
	glutSetWindow(windowFPV);

}


void keyboardSpecial(int key, int x, int y)
{
	idleTime = 0;

	switch (key) {


	case GLUT_KEY_LEFT: {
		blimp->setRotation(blimp->getRotation() + 3);
	//	blimp->addLocation(0, 0, 0);
	  }
	break;

	case GLUT_KEY_RIGHT: {
		blimp->setRotation(blimp->getRotation() - 3);
		//blimp->addLocation(0, 0, 0);
	  }
	break;

	case GLUT_KEY_UP: {
		blimp->addLocation(0, 0.1, 0);
	}

	break;

	case GLUT_KEY_DOWN: {
		blimp->addLocation(0, -0.1, 0);
	}

	}

	// Trigger a window redisplay
	glutPostRedisplay();
	glutSetWindow(windowBird);
	glutPostRedisplay();
	glutSetWindow(windowFPV);
}



void displayBird(void)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	


	loadIdentityCustom();

	// Bird's Eye View
	float viewHeight = 50.0;
	float blimpX = blimp->getX();
	float blimpZ = blimp->getZ();
	gluLookAt(blimpX, viewHeight, blimpZ, blimpX, 0.0, blimpZ, 0.0, 0.0, 1.0);
	
	
	//glLoadIdentity();

	ground->draw();
	blimp->draw();

	blimp2->draw();
	blimp2->tick();

	glutSwapBuffers();   // Double buffering, swap buffers
}


void displayIdleBird() { 
	displayBird();
}



void displayFPV(void)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



	loadIdentityCustom();

	float eyeX; float eyeY; float eyeZ;
	float centerX; float centerY; float centerZ;

	blimp->cameraPosition(&eyeX, &eyeY, &eyeZ, &centerX, &centerY, &centerZ);

	//FPV View
	gluLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, 0.0, 1.0, 0.0);

	//glLoadIdentity();

	ground->draw();
	blimp->draw();

	blimp2->draw();
	blimp2->tick();

	glutSwapBuffers();   // Double buffering, swap buffers
}


void displayIdleFPV() {
	displayFPV();
}
