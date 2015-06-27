#include <iostream>


#include <glut.h> 
#include <GL/gl.h>  
#include <GL/glu.h>  
#include <math.h>	// TODO: search for C++ math.h alike
//#include "tga.h"
#include "TGALoader.h"
#include "Logger.h"
#include "Camera.h"
#include "Level.h"



using namespace std;

void init(int width, int height);	// TODO: Create Main Class and not this c like bull****
void resize(int width, int height);
void keyPressed(unsigned char key, int x, int y);
void keyReleased(unsigned char key, int x, int y);
void display();
void drawCube();
void timer(int value);
void mouse(int button, int state, int x, int y);
void mouseMotion(int x, int y);
void reportGLError(const char * msg);
void loadTexture();
void timerWarpAnimation(int Recursion);
bool BounceUpwards = true;

int moving = 0;			/* flag that is true while mouse moves */
int begin_x = 0;        /* x value of mouse movement */
int begin_y = 0;		/* y value of mouse movement */

int window = 0;
float DeltaMovementUpDown = 0.0f;

Logger* mLogger = Logger::getInstance();
Camera* mCamera = Camera::getInstance();
Level* mLevel = Level::getInstance();

int main(int argc, char **argv)
{
	mLogger->LogInfo("Programm started");

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
	glutInitWindowSize(640, 480);
	glutInitWindowPosition(0, 0);
	window = glutCreateWindow("Labyrinth");

	glutSetCursor(GLUT_CURSOR_NONE);	// so there is no cursor being displayed

	glutDisplayFunc(&display);
	glutReshapeFunc(&resize);
	glutKeyboardFunc(&keyPressed);
	glutKeyboardUpFunc(&keyReleased);

	init(640, 480);
	glutTimerFunc(15, timer, 1);

	glutMouseFunc(mouse);
	glutMotionFunc(mouseMotion);
	glutPassiveMotionFunc(mouseMotion);

	glutFullScreen();
	glutMainLoop();
	return 0;
}

void init(int width, int height)
{
	glClearColor(0.4980392156862745f, 0.7372549019607843f, 1, 0.0f);
	glClearDepth(1.0);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);

	resize(width, height);

	TGALoader wallLoader;
	wallLoader.loadTexture("./../Labyrinth/Textures/crate.tga");
	mLevel->setFloorTextureId(wallLoader.getTextureId());

	TGALoader floorLoader;
	floorLoader.loadTexture("./../Labyrinth/Textures/ground2.tga");
	mLevel->setWallTextureId(floorLoader.getTextureId());


}

void resize(int width, int height)
{
	// prevent division by zero
	if (height == 0) 
		height = 1;

	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (float)width / (float)height, 0.1f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
}

void keyPressed(unsigned char key, int x, int y)
{
	switch (key) {
	case 27:
		glutDestroyWindow(window);
		exit(0);
		break;

	case 'w':
		//mCamera->moveForward();
		mCamera->modifyKeyStatus('w', true);
		break;

	case 'a':
		//mCamera->moveLeft();
		mCamera->modifyKeyStatus('a', true);
		break;

	case 's':
		//mCamera->moveBack();
		mCamera->modifyKeyStatus('s', true);
		break;

	case 'd':
		//mCamera->moveRight();
		mCamera->modifyKeyStatus('d', true);
		break;

	case 'c':
		mCamera->logViewStats();
		break;

	default:
		break;
	}
}

void keyReleased(unsigned char key, int x, int y)
{
	switch(key) 
	{
		case 'w':
			//mCamera->moveForward();
			mCamera->modifyKeyStatus('w', false);
			break;

		case 'a':
			//mCamera->moveLeft();
			mCamera->modifyKeyStatus('a', false);
			break;

		case 's':
			//mCamera->moveBack();
			mCamera->modifyKeyStatus('s', false);
			break;

		case 'd':
			//mCamera->moveRight();
			mCamera->modifyKeyStatus('d', false);
			break;

		default:
			break;
	}
}



void mouse(int button, int state, int x, int y)
{
	switch (button) {
	case GLUT_LEFT_BUTTON:    /* spin scene around */
		if (state == GLUT_DOWN){
			moving = 1;
			begin_x = x;
			begin_y = y;

		}
		//else if (state == GLUT_UP){
		//	moving = 0;
		//}
		break;

	default:
		break;
	}

	glutPostRedisplay();
}


void mouseMotion(int x, int y)
{

	mCamera->calculateViewAngle(x, y);

	glutPostRedisplay();
}


void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	//gluLookAt(mCamera->getViewCoordX(), mCamera->getViewCoordY(), mCamera->getViewCoordZ(),	// ersten 3 Werte = x,y,z von Aug-Punkt - "dort wo ich hinschau"
	//	0., 0., 0.,		// center point
	//	0., 1., 0.);	// letzten werte = "up pointer" -> rauf is bei uns rauf = y Achse
	
	mCamera->updateMovement();
	if (mLevel->checkLevelFinished())
		glutTimerFunc(200, &timerWarpAnimation, 10);

	glRotatef(mCamera->getAngleX(), 1, 0, 0);
	glRotatef(mCamera->getAngleY(), 0, 1, 0);
	glTranslatef(-mCamera->getViewCoordX(), -mCamera->getViewCoordY(), -mCamera->getViewCoordZ());	// das ist das vor- zurück gehen. gehen verändert hier die welt und nicht den augpunkt

	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);	// Environment mode -> bei GL_MODULATE wird defaultmäßig multipliziert; bei GL_BLEND wird "geblendet"
		
	mLevel->renderLevel();

	glDisable(GL_TEXTURE_2D);

	glutSwapBuffers();
}

void timer(int value)
{
	glutPostRedisplay();
	glutTimerFunc(15, timer, 1);
}

void timerWarpAnimation(int Recursion)
{
	if (Recursion > 0)
	{
		if (BounceUpwards)
		{
			mCamera->modifyViewCoordY(-0.005f);
			if (mCamera->getViewCoordY() < -0.5)
				BounceUpwards = false;
		}
		else
		{
			mCamera->modifyViewCoordY(0.005f);
			if (mCamera->getViewCoordY() > 0)
				BounceUpwards = true;
		}

		glutPostRedisplay();
		glutTimerFunc(200, &timerWarpAnimation, --Recursion);
	}
	else
	{
		/* Nach Ablauf der Zeit das Programm beenden */
		glutDestroyWindow(window);
		exit(0);
	}
}



