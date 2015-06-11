#pragma once
#include <windows.h>              // Standard Windows header
#include <stdio.h>
#include <GL\gl.h>                // Standard Header For OpenGL

#include "Logger.h"
class TGALoader
{
public:
	typedef struct
	{
		GLubyte* imageData;         // Hold All The Color Values For The Image.
		GLuint  bpp;                // Hold The Number Of Bits Per Pixel.          
		GLuint width;               // The Width Of The Entire Image.  
		GLuint height;              // The Height Of The Entire Image. 
		GLuint texID;               // Texture ID For Use With glBindTexture.  
		GLuint type;                // Data Stored In * ImageData (GL_RGB Or GL_RGBA)
	} Texture;

	typedef struct
	{
		GLubyte Header[12];         // File Header To Determine File Type
	} TGAHeader;

	typedef struct
	{
		GLubyte header[6];          // Holds The First 6 Useful Bytes Of The File
		GLuint bytesPerPixel;           // Number Of BYTES Per Pixel (3 Or 4)
		GLuint imageSize;           // Amount Of Memory Needed To Hold The Image
		GLuint type;                // The Type Of Image, GL_RGB Or GL_RGBA
		GLuint Height;              // Height Of Image                 
		GLuint Width;               // Width Of Image              
		GLuint Bpp;             // Number Of BITS Per Pixel (24 Or 32)
	} TGA;

	TGALoader();
	bool LoadTGA(Texture * texture, char * filename);

private:
	TGAHeader tgaheader;        // Used To Store Our File Header
	TGA tga;                    // Used To Store File Information

	static GLubyte uTGAcompare[12];
	static GLubyte cTGAcompare[12];
	Logger *mLogger;

	bool LoadUncompressedTGA(Texture *, char *, FILE *);
	bool LoadCompressedTGA(Texture *, char *, FILE *);
	
};