#pragma once
#include <windows.h>              // Standard Windows header
#include <stdio.h>
#include <GL\gl.h>                // Standard Header For OpenGL
#include <glut.h> 
#include <GL/glu.h>  
#include "Logger.h"

#define TGA_ERROR_FILE_OPEN          -5
#define TGA_ERROR_READING_FILE       -4
#define TGA_ERROR_INDEXED_COLOR      -3
#define TGA_ERROR_MEMORY             -2
#define TGA_ERROR_COMPRESSED_FILE    -1
#define TGA_OK                        0

class TGALoader
{
public:
	TGALoader();
	typedef struct 
	{
		int status;
		unsigned char type;
		unsigned char pixelDepth;
		short int width;
		short int height;
		unsigned char *imageData;
	} 
	tgaInfo;

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

	void loadTexture(std::string Filename);
	GLuint getTextureId();

	GLuint TextureId;

#ifndef _WIN32 
	static void swap_16bit_word(uint16_t *word)
#endif

private:
	void tgaLoadHeader(FILE *file, tgaInfo *info);
	void tgaLoadImageData(FILE *file, tgaInfo *info);
	Logger *mLogger;

	void reportGLError(const char * msg);

	
};