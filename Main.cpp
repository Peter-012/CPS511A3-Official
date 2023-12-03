
/*
 CPS511 A1 Part 1

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



GroundMesh* ground = new GroundMesh(16, 0, -20, 5);
Blimp* blimp = new Blimp(0,-1,-10);
Enemy* blimp2 = new Enemy(0, -5, -10, blimp);
Missile* missile1;

bool firePlayerMissile = false;
bool drawMissile = false;
int reloadTime = 500;
int missileTimer = reloadTime + 1;

int idleTime = 0;

const float PI = 3.14159265;


// Default Mesh Size
int meshSize = 16;

// Prototypes for functions in this module

void displayIdle(void);
void keyboard(unsigned char key, int x, int y);
void keyboardSpecial(int key, int x, int y);
void display();





int main(int argc, char **argv)
{
	// Initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(VIEWPORT_WIDTH, VIEWPORT_HEIGHT);
	glutInitWindowPosition(200, 30);
	glutCreateWindow("Blimp");

	// Initialize GL
	initOpenGL(VIEWPORT_WIDTH, VIEWPORT_HEIGHT);

	// Register callback functions
	glutDisplayFunc(display);

	glutReshapeFunc(reshape);
	glutSpecialFunc(keyboardSpecial);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(displayIdle);
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

	case ' ': {
		if (missileTimer > reloadTime) {
			firePlayerMissile = true;
			missileTimer = 0;
		}
	}
	break;
	}

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

	glutPostRedisplay();   // Trigger a window redisplay
}



void display(void)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	


	loadIdentityCustom();
	gluLookAt(0.0, 10.0, 30.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	//glLoadIdentity();

	ground->draw();
	blimp->draw();

	blimp2->draw();
	blimp2->tick();

	if (firePlayerMissile) {
		float blimpBottom = blimp->getBottomBlimpY();
		float missileRotation = -1 * toRadians(blimp->getRotation());

		float missileX = blimp->getX();
		float missileY = blimp->getY() + blimpBottom;
		float missileZ = blimp->getZ();

		missile1 = new Missile(missileX, missileY, missileZ, missileRotation);

		firePlayerMissile = false;
		drawMissile = true;
	}
	else if (drawMissile && missileTimer < reloadTime + 1) {
		missile1->draw();
		missileTimer++;
	}

	glutSwapBuffers();   // Double buffering, swap buffers
}


void displayIdle() { 
	display();
}




