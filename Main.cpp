
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
//#include "header/Missile.h"
#include "header/Util.h"
#include "header/RayTrace.h"
#include "header/ObjectPositionHandler.h"

const int VIEWPORT_WIDTH  = 800;    // Viewport width in pixels
const int VIEWPORT_HEIGHT = 600;    // Viewport height in pixels



GroundMesh* ground = new GroundMesh(16, 0, -20, 5);
Blimp* blimp = new Blimp(0,-1,-10);
Enemy* blimp2 = new Enemy(0, 0, 0, blimp);

ObjectPositionHandler* handler = new ObjectPositionHandler();


//Missile* missile1;

bool firePlayerLazer = false;

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
	handler->setPlayer(blimp);
	handler->setEnemy(blimp2);


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
	case 's': {
		blimp->moveForward(-0.5, -0.5);
		//blimp2->moveForward(0.2, 0.2);
	}
			break;



	case ' ': {
		if (!firePlayerLazer) {

			BoundingBox eBox = blimp2->getCurrentBoundingBoxState();


			//the -2 for the y is to account for the fact that the lazer is below
			VECTOR3D* playerLoc = new VECTOR3D(blimp->getX(), blimp->getY() - 2, blimp->getZ());
			VECTOR3D playerDir = blimp->getDirection();
			HitResult hitEnemy = rayTrace(&eBox, playerLoc, &playerDir);

			bool hasHitEnemy = hitEnemy.hitEntity;

			if (hasHitEnemy) {
				
				VECTOR3D hitLoc = hitEnemy.hitPos;
				float diffX = hitLoc.GetX() - playerLoc->GetX();
				float diffZ = hitLoc.GetZ() - playerLoc->GetZ();
			
				float distance = sqrt(diffX * diffX + diffZ * diffZ);
				blimp->setLazerScale(distance);
				blimp->setLazerScaleUpdated(true);
			}
			else {
				blimp->setLazerScaleUpdated(false);
			}



			blimp->fireLazer();
			firePlayerLazer = true;
		} else {
			blimp->stopLazer();
			firePlayerLazer = false;
		}	
	}
	break;
	}
	glutPostRedisplay();
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

	float eyeX; float eyeY; float eyeZ;
	float centerX; float centerY; float centerZ;

	blimp->cameraPosition(&eyeX, &eyeY, &eyeZ, &centerX, &centerY, &centerZ);

	//FPV View
	//gluLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, 0.0, 1.0, 0.0);

	// Bird's Eye View
	float viewHeight = 50.0;
	gluLookAt(0.0, viewHeight, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);


	//default view
	//gluLookAt(0.0, 6.0, 22.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	
	
	//glLoadIdentity();

	ground->draw();
	blimp->draw();

	blimp2->draw();
	blimp2->tick();

	glutSwapBuffers();   // Double buffering, swap buffers
}


void displayIdle() { 
	display();
}




