#include <iostream>


#include <glut.h> 
#include <GL/gl.h>  
#include <GL/glu.h>  
#include <math.h>	// TODO: search for C++ math.h alike
//#include "tga.h"
#include "TGALoader.h"
#include "Logger.h"


#define RAD(x) (((x)*M_PI)/180.)	// TODO: Create method in Main Class

/* some math.h files don't define pi... */
#ifndef M_PI
#define M_PI 3.141592653
#endif



using namespace std;

void init(int width, int height);	// TODO: Create Main Class and not this c like bull****
void resize(int width, int height);
void specialKeyPressed(int key, int x, int y);
void keyPressed(unsigned char key, int x, int y);
void display();
void drawCube();
void timer(int value);
void mouse(int button, int state, int x, int y);
void mouseMotion(int x, int y);
void reportGLError(const char * msg);
void loadTexture();
void RenderScene();


int animating = 1;		// TODO: What is this doing? Make it non global

//Camera:
GLfloat angle_y = 0;	/* angle of spin around y axis of scene, in degrees */
GLfloat angle_x = 0;	/* angle of spin around x axis  of scene, in degrees */
float x = 0.0f, z = 5.0f, y = 1.75f;	// XZ position of the camera
float lx = 0.0f, lz = -1.0f, ly = 0.0f;	// defining the cameras position
float deltaAngle = 0.0f;
float deltaMove = 0;

int moving = 0;			/* flag that is true while mouse moves */
int begin_x = 0;        /* x value of mouse movement */
int begin_y = 0;		/* y value of mouse movement */

int window = 0;
float DeltaMovementUpDown = 0.0f;

TGALoader *mLoader;
Logger* mLogger = Logger::getInstance();

int main(int argc, char **argv)
{
	mLogger->LogInfo("Programm started");
	mLoader = new TGALoader();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
	glutInitWindowSize(640, 480);
	glutInitWindowPosition(0, 0);
	window = glutCreateWindow("Labyrinth");

	glutSetCursor(GLUT_CURSOR_NONE);	// so there is no cursor being displayed

	glutDisplayFunc(&display);
	glutReshapeFunc(&resize);
	glutKeyboardFunc(&keyPressed);
	glutSpecialFunc(&specialKeyPressed);

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
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);

	resize(width, height);

	mLoader->loadTexture("crate.tga");
}

void computePos(float deltaMove) 
{

	x += deltaMove * lx * 0.1f;
	z += deltaMove * lz * 0.1f;
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

void specialKeyPressed(int key, int x, int y)
{
	switch (key) {

	case GLUT_KEY_UP:     /* <cursor up> */
		DeltaMovementUpDown += 0.1f;
		glutPostRedisplay();
		break;
	case GLUT_KEY_DOWN:     /* <cursor down> */
		DeltaMovementUpDown -= 0.1f;
		glutPostRedisplay();
		break;
	}
}

void keyPressed(unsigned char key, int x, int y)
{
	switch (key) {
	case 27:
		glutDestroyWindow(window);
		exit(0);
		break;
	case 'a':
		animating = animating ? 0 : 1;
		glutPostRedisplay();
		break;

	case 'w':
		DeltaMovementUpDown += 0.1f;
		break;

	case 's':
		DeltaMovementUpDown -= 0.1f;
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
	/* calculate new modelview matrix values */
	angle_y = angle_y + (x - begin_x);	// aktuelle x coord - x coordinate von beginn
	angle_x = angle_x + (y - begin_y);	// wenn ich mich nach links rechts drehen will -> drehung um y Achse -> daher sind hier x und y "vertauscht"

	if (angle_x > 360.0)
		angle_x -= 360.0;
	else if (angle_x < -360.0)
		angle_x += 360.0;

	if (angle_y > 360.0)
		angle_y -= 360.0;
	else if (angle_y < -360.0)
		angle_y += 360.0;

	begin_x = x;
	begin_y = y;
	
	glutPostRedisplay();
}





void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	gluLookAt(-sinf(RAD(angle_y)), sinf(RAD(angle_x)), cosf(RAD(angle_y)),	// ersten 3 Werte = x,y,z von Aug-Punkt - "dort wo ich hinschau"
		0., 0., 0.,	// mit sinf rad bla werden die koordinaten aus den winkel errechnet; 2. 3 Werte = center point
		0., 1., 0.);	// letzten werte = "up pointer" -> rauf is bei uns rauf = y Achse

	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);	// Environment mode -> bei GL_MODULATE wird defaultmäßig multipliziert; bei GL_BLEND wird "geblendet"
	
	glBindTexture(GL_TEXTURE_2D, mLoader->getTextureId());

	glTranslatef(0, 0, -DeltaMovementUpDown);	// das ist das vor- zurück gehen. gehen verändert hier die welt und nicht den augpunkt
	RenderScene();


	glDisable(GL_TEXTURE_2D);

	glutSwapBuffers();
}

void RenderScene()
{
	//common practice - ursprungspunkt verschieben und objekte auf ursprungspunkt zeichnen
	glPushMatrix();
	glTranslatef(-2, 0, 0);
	glTranslatef(0, 0, 4);	// linke reihe ganz vorne, und dann immer -4 nach hinten (oder umgekehrt)
	drawCube();
	glTranslatef(0, 0, -4);
	drawCube();
	glTranslatef(0, 0, -4);
	drawCube();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(2, 0, 0);
	glTranslatef(0, 0, 4);
	drawCube();
	glTranslatef(0, 0, -4);
	drawCube();
	glTranslatef(0, 0, -4);
	drawCube();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(6, 0, 0);
	glTranslatef(0, 0, 4);
	drawCube();
	glTranslatef(0, 0, -4);
	drawCube();
	glTranslatef(0, 0, -4);
	drawCube();
	glPopMatrix();
}

void drawCube()
{
	//Die Kisten textur ist eine "hoch-frequente" Textur -> flimmert
	//hoch frequenz deswegen weil die textur ja ein 2d array ist -> kann man als funktion aufzeichnen -> ist ein signal -> hat eine frequenz
	//das flackern bekommt man mit "mip - mapping" weg -> ist heute common practice
	glBegin(GL_QUADS);
	// front face
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	// back face
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	// top face
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	// bottom face
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	// right face
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	// left face
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glEnd();
}

void timer(int value)
{
	glutPostRedisplay();
	glutTimerFunc(15, timer, 1);
}




