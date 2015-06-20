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

typedef struct {
	int status;
	unsigned char type, pixelDepth;
	short int width, height;
	unsigned char *imageData;
} tgaInfo;
tgaInfo* tgaLoad(char *filename);

int tgaSave(char *filename,
	short int width,
	short int height,
	unsigned char pixelDepth,
	unsigned char *imageData);

int tgaSaveSeries(char *filename,
	short int width,
	short int height,
	unsigned char pixelDepth,
	unsigned char *imageData);

void tgaRGBtoGreyscale(tgaInfo *info);

int tgaGrabScreenSeries(char *filename, int x, int y, int w, int h);

void tgaDestroy(tgaInfo *info);
#define TGA_ERROR_FILE_OPEN          -5
#define TGA_ERROR_READING_FILE       -4
#define TGA_ERROR_INDEXED_COLOR      -3
#define TGA_ERROR_MEMORY             -2
#define TGA_ERROR_COMPRESSED_FILE    -1
#define TGA_OK                        0

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

int animating = 1;		// TODO: What is this doing? Make it non global

GLfloat angle_y = 0;	/* angle of spin around y axis of scene, in degrees */
GLfloat angle_x = 0;	/* angle of spin around x axis  of scene, in degrees */

int moving = 0;			/* flag that is true while mouse moves */
int begin_x = 0;        /* x value of mouse movement */
int begin_y = 0;		/* y value of mouse movement */

GLuint texture;
int window = 0;
float DeltaMovement = 0.0f;

Logger* mLogger = Logger::getInstance();

int main(int argc, char **argv)
{
	mLogger->LogInfo("Programm started");

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
	glutInitWindowSize(640, 480);
	glutInitWindowPosition(0, 0);
	window = glutCreateWindow("Labyrinth");

	glutDisplayFunc(&display);
	glutReshapeFunc(&resize);
	glutKeyboardFunc(&keyPressed);
	glutSpecialFunc(&specialKeyPressed);

	init(640, 480);
	glutTimerFunc(15, timer, 1);
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMotion);
	glutFullScreen();
	glutMainLoop();
	return 0;
}

void init(int width, int height)
{
	//loadTexture();

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);

	resize(width, height);

	//load texture with c loader
	GLsizei w, h;
	tgaInfo *info = 0;
	int mode;

	info = tgaLoad("crate.tga");

	if (info->status != TGA_OK) {
		fprintf(stderr, "error loading texture image: %d\n", info->status);

		return;
	}
	if (info->width != info->height) {
		fprintf(stderr, "Image size %d x %d is not rectangular, giving up.\n",
			info->width, info->height);
		return;
	}

	mode = info->pixelDepth / 8;  // will be 3 for rgb, 4 for rgba
	glGenTextures(1, &texture);	// Es wird eine ID für die Textur generiert. texture ist eine globale var

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glBindTexture(GL_TEXTURE_2D, texture);	// alle befehle die nachher kommen, sollen mit dieser ID verknüpft werden
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// geometrie einpacken - S Achse = u Achse = x Achse
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);	// geometrie einpacken - T Achse = v Achse = Y Achse
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	// Magnification - wird beim Skalieren verwendet (Geometrie ändert sich, Kamera ändert sich, Textur pass nicht genau drauf)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// Minification, hier wird auch ein Filter definiert
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

	// Upload the texture bitmap. 
	w = info->width;
	h = info->height;

	reportGLError("before uploading texture");
	GLint format = (mode == 4) ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format,	// bei RGB Bild hat jedes pixel 3 (RGB) ein 1 Byte -> daher auch unsigned byte
		GL_UNSIGNED_BYTE, info->imageData);
	reportGLError("after uploading texture");	// Wenn der Befehl vorbei ist, dann ist die Textur auf der GraKa

	tgaDestroy(info);	// jetzt hab ich die Textur 2 mal -> einmal im GraKa Speicher einmal im RAM -> aus RAM löschen

}


void loadTexture()
{
	TGALoader::Texture texture;	// holds information about the loaded TGA texture

	// Load The Bitmap, Check For Errors.
	TGALoader Loader;
	
	
	if (Loader.LoadTGA(&texture, "crate.tga"))
	{
		glGenTextures(1, &texture.texID);				// Create The Texture ( CHANGE )
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glBindTexture(GL_TEXTURE_2D, texture.texID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

		//TODO: use mipmaps:	https://open.gl/textures
		
		//glTexImage2D(GL_TEXTURE_2D, 0, texture.bpp / 8, texture.width, texture.height, 0, texture.type, GL_UNSIGNED_BYTE, texture.imageData);
		// Black/white checkerboard
		float pixels[] = {
			0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f
		};
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_FLOAT, pixels);	// black and white checkboard texture for testing purposes

		if (texture.imageData)						// If Texture Image Exists ( CHANGE )
		{
			free(texture.imageData);					// Free The Texture Image Memory ( CHANGE )
		}
	}
}


void resize(int width, int height)
{
	// prevent division by zero
	if (height == 0) { height = 1; }

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
		DeltaMovement += 0.1f;
		glutPostRedisplay();
		break;
	case GLUT_KEY_DOWN:     /* <cursor down> */
		DeltaMovement -= 0.1f;
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
	default:
		break;
	}
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

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();



	gluLookAt(-sinf(RAD(angle_y)), sinf(RAD(angle_x)), cosf(RAD(angle_y)),	// ersten 3 Werte = x,y,z von Aug-Punkt - "dort wo ich hinschau"
		0., 0., 0.,	// mit sinf rad bla werden die koordinaten aus den winkel errechnet; 2. 3 Werte = center point
		0., 1., 0.);	// letzten werte = "up pointer" -> rauf is bei uns rauf = y Achse

	//glTranslatef(0, 0, -4);
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);	// Environment mode -> bei GL_MODULATE wird defaultmäßig multipliziert; bei GL_BLEND wird "geblendet"
	glBindTexture(GL_TEXTURE_2D, texture);

	glTranslatef(0, 0, -DeltaMovement);	// das ist das vor- zurück gehen. gehen verändert hier die welt und nicht den augpunkt

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

	glDisable(GL_TEXTURE_2D);

	glutSwapBuffers();
}

void timer(int value)
{
	glutPostRedisplay();
	glutTimerFunc(15, timer, 1);
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
		else if (state == GLUT_UP){
			moving = 0;
		}
		break;

	default:
		break;
	}

	glutPostRedisplay();
}


void mouseMotion(int x, int y) 
{

	if (moving) /* mouse button is pressed */
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
}

void reportGLError(const char * msg)
{
	GLenum errCode;
	const GLubyte *errString;
	while ((errCode = glGetError()) != GL_NO_ERROR) {
		errString = gluErrorString(errCode);
		fprintf(stderr, "OpenGL Error: %s %s\n", msg, errString);
	}
	return;
}


//start do delete here:
// Based on public domain code by Antonio R. Fernandes <ajbrf@yahoo.com>

#define _CRT_SECURE_NO_WARNINGS 1

#ifdef _WIN32
#include <windows.h>
#endif


#ifdef __APPLE__
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else 
#include <GL/glu.h>
#include <glut.h>
#endif


#ifdef WIN32
#include <windows.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef _WIN32
#include <stdint.h>
#endif


#include "tga.h"

// this variable is used for image series
static int savedImages = 0;

#ifndef _WIN32 
static void
swap_16bit_word(uint16_t *word)
{
	uint8_t *block = (uint8_t *)word;
	uint8_t tmp;
	tmp = block[1];
	block[1] = block[0];
	block[0] = tmp;
}
#endif

// load the image header fields. We only keep those that matter!
void tgaLoadHeader(FILE *file, tgaInfo *info) {

	unsigned char cGarbage;
	short int iGarbage;

	fread(&cGarbage, sizeof(unsigned char), 1, file);
	fread(&cGarbage, sizeof(unsigned char), 1, file);

	// type must be 2 or 3
	fread(&info->type, sizeof(unsigned char), 1, file);

	fread(&iGarbage, sizeof(short int), 1, file);
	fread(&iGarbage, sizeof(short int), 1, file);
	fread(&cGarbage, sizeof(unsigned char), 1, file);
	fread(&iGarbage, sizeof(short int), 1, file);
	fread(&iGarbage, sizeof(short int), 1, file);

	fread(&info->width, sizeof(short int), 1, file);
	fread(&info->height, sizeof(short int), 1, file);
	fread(&info->pixelDepth, sizeof(unsigned char), 1, file);

#ifdef __APPLE__
	// FIXME: Determine byte order at runtime.
	swap_16bit_word((uint16_t*)&info->width);
	swap_16bit_word((uint16_t*)&info->height);
#endif // __APPLE__

#if 0 // debug
	printf("Image dimensions: %d %d %d\n",
		info->width, info->height, info->pixelDepth);
#endif

	fread(&cGarbage, sizeof(unsigned char), 1, file);
}

// loads the image pixels. You shouldn't call this function
// directly
void tgaLoadImageData(FILE *file, tgaInfo *info) {

	int mode, total, i;
	unsigned char aux;

	// mode equal the number of components for each pixel
	mode = info->pixelDepth / 8;
	// total is the number of bytes we'll have to read
	total = info->height * info->width * mode;

	fread(info->imageData, sizeof(unsigned char), total, file);

#if 0 // debug
	printf("Image mode: %d\n", mode);
#endif

	// mode=3 or 4 implies that the image is RGB(A). However TGA
	// stores it as BGR(A) so we'll have to swap R and B.
	if (mode >= 3)
	for (i = 0; i < total; i += mode) {
#if 1 // original code
		aux = info->imageData[i];
		info->imageData[i] = info->imageData[i + 2];
		info->imageData[i + 2] = aux;
#else // somehow I had a texture file which was RBG (!) 
		aux = info->imageData[i + 1];
		info->imageData[i + 1] = info->imageData[i + 2];
		info->imageData[i + 2] = aux;
#endif
	}
}

// this is the function to call when we want to load
// an image
tgaInfo * tgaLoad(char *filename)
{

	FILE *file;
	tgaInfo *info;
	int mode, total;

	// allocate memory for the info struct and check!
	info = (tgaInfo *)malloc(sizeof(tgaInfo));
	if (info == NULL)
		return(NULL);


	// open the file for reading (binary mode)
#pragma warning (disable : 4996)
	file = fopen(filename, "rb");
#pragma warning(default: 4996)
	if (file == NULL) {
		info->status = TGA_ERROR_FILE_OPEN;
		return(info);
	}

	clearerr(file);

	// load the header
	tgaLoadHeader(file, info);

	// check for errors when loading the header
	if (ferror(file)) {
		info->status = TGA_ERROR_READING_FILE;
		fprintf(stderr, "error reading file\n");
		fclose(file);
		return(info);
	}

	// check if the image is color indexed
	if (info->type == 1) {
		info->status = TGA_ERROR_INDEXED_COLOR;
		fclose(file);
		return(info);
	}

#if 0
	// check for other types (compressed images)
	if ((info->type != 2) && (info->type != 3)) {
		info->status = TGA_ERROR_COMPRESSED_FILE;
		fprintf(stderr, "error: compressed file (type was %d)\n", info->type);
		fclose(file);
		return(info);
	}
#endif

	// mode equals the number of image components
	mode = info->pixelDepth / 8;
	// total is the number of bytes to read
	total = info->height * info->width * mode;
	// allocate memory for image pixels
	info->imageData = (unsigned char *)malloc(sizeof(unsigned char)*
		total);

	// check to make sure we have the memory required
	if (info->imageData == NULL) {
		info->status = TGA_ERROR_MEMORY;
		fclose(file);
		return(info);
	}
	// finally load the image pixels
	tgaLoadImageData(file, info);

	// check for errors when reading the pixels
	if (ferror(file)) {
		info->status = TGA_ERROR_READING_FILE;
		fclose(file);
		return(info);
	}
	fclose(file);
	info->status = TGA_OK;
	return(info);
}

// converts RGB to greyscale
void tgaRGBtoGreyscale(tgaInfo *info) {

	int mode, i, j;

	unsigned char *newImageData;

	// if the image is already greyscale do nothing
	if (info->pixelDepth == 8)
		return;

	// compute the number of actual components
	mode = info->pixelDepth / 8;

	// allocate an array for the new image data
	newImageData = (unsigned char *)malloc(sizeof(unsigned char)*
		info->height * info->width);
	if (newImageData == NULL) {
		return;
	}

	// convert pixels: greyscale = o.30 * R + 0.59 * G + 0.11 * B
	for (i = 0, j = 0; j < info->width * info->height; i += mode, j++)
		newImageData[j] = (unsigned char)(0.30 * info->imageData[i] +
		0.59 * info->imageData[i + 1] +
		0.11 * info->imageData[i + 2]);


	//free old image data
	free(info->imageData);

	// reassign pixelDepth and type according to the new image type
	info->pixelDepth = 8;
	info->type = 3;
	// reassing imageData to the new array.
	info->imageData = newImageData;
}

// takes a screen shot and saves it to a TGA image
int tgaGrabScreenSeries(char *filename, int x, int y, int w, int h) {

	unsigned char *imageData;

	// allocate memory for the pixels
	imageData = (unsigned char *)malloc(sizeof(unsigned char)* w * h * 4);

	// read the pixels from the frame buffer
	glReadPixels(x, y, w, h, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid *)imageData);

	// save the image 
	return(tgaSaveSeries(filename, w, h, 32, imageData));
}

// saves an array of pixels as a TGA image
int tgaSave(char            *filename,
	short int        width,
	short int        height,
	unsigned char    pixelDepth,
	unsigned char    *imageData) {

	unsigned char cGarbage = 0, type, mode, aux;
	short int iGarbage = 0;
	int i;
	FILE *file;

	// open file and check for errors
	file = fopen(filename, "wb");
	if (file == NULL) {
		return(TGA_ERROR_FILE_OPEN);
	}

	// compute image type: 2 for RGB(A), 3 for greyscale
	mode = pixelDepth / 8;
	if ((pixelDepth == 24) || (pixelDepth == 32))
		type = 2;
	else
		type = 3;

	// write the header
	fwrite(&cGarbage, sizeof(unsigned char), 1, file);
	fwrite(&cGarbage, sizeof(unsigned char), 1, file);

	fwrite(&type, sizeof(unsigned char), 1, file);

	fwrite(&iGarbage, sizeof(short int), 1, file);
	fwrite(&iGarbage, sizeof(short int), 1, file);
	fwrite(&cGarbage, sizeof(unsigned char), 1, file);
	fwrite(&iGarbage, sizeof(short int), 1, file);
	fwrite(&iGarbage, sizeof(short int), 1, file);

	fwrite(&width, sizeof(short int), 1, file);
	fwrite(&height, sizeof(short int), 1, file);
	fwrite(&pixelDepth, sizeof(unsigned char), 1, file);

	fwrite(&cGarbage, sizeof(unsigned char), 1, file);

	// convert the image data from RGB(a) to BGR(A)
	if (mode >= 3)
	for (i = 0; i < width * height * mode; i += mode) {
		aux = imageData[i];
		imageData[i] = imageData[i + 2];
		imageData[i + 2] = aux;
	}

	// save the image data
	fwrite(imageData, sizeof(unsigned char), width * height * mode, file);
	fclose(file);
	// release the memory
	free(imageData);

	return(TGA_OK);
}

// saves a series of files with names "filenameX.tga"
int tgaSaveSeries(char        *filename,
	short int        width,
	short int        height,
	unsigned char    pixelDepth,
	unsigned char    *imageData) {

	char *newFilename;
	int status;
	// compute the new filename by adding the series number and the extension

	newFilename = (char *)malloc(sizeof(char)* strlen(filename) + 8);

	sprintf(newFilename, "%s%d.tga", filename, savedImages);
	// save the image
	status = tgaSave(newFilename, width, height, pixelDepth, imageData);
	//increase the counter
	savedImages++;
	return(status);
}


// releases the memory used for the image
void tgaDestroy(tgaInfo *info) {

	if (info != NULL) {
		free(info->imageData);
		free(info);
	}
}
