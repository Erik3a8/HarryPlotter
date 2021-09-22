/* Base class for all types of Canvas, always a 2D grid filled with RGB pixels. Contains a method to output as .bmp file.
For animations I have to impose a size limit because mpeg is somehow limited. */

#ifndef BASECANVAS_H
#define BASECANVAS_H

#include "Function.h"
#include "Colors.h"


class BaseCanvas {
private:

	// Elements of my Canvas 
	static unsigned int height;																					
	static unsigned int width;
	
	// Make sure this is NEVER set to zero!
	static double resolution;

	// Center Point of the canvas
	static double centerX;
	static double centerY;

	static std::string PATH; 


	// For my .bmp file
	int BYTES_PER_PIXEL = 3; /// red, green, & blue
	int FILE_HEADER_SIZE = 14;
	int INFO_HEADER_SIZE = 40;

public:

	// This points to what will contain all data, note we have an array of color objects. 
	Color* buffer = nullptr;

	// Those are just here to increase readibility and user friendliness
	double realSizeX = width * resolution;
	double realSizeY = height * resolution;

	// Access to Private members
	static unsigned int getHeight ();
	static unsigned int getWidth ();

	static double getCenterX ();
	static double getCenterY ();
	static double getResolution ();
	static std::string getPath();

	Color* getBuffer ();

	// Modify Private Members
	static void setHeight (unsigned int h);
	static void setWidth (unsigned int w);
	static void setCenterX (double CX);
	static void setCenterY (double CY);
	static void setResolution (double res);
	void setPixel (unsigned int x, unsigned int y, const Color Pixel);
	static void setPath (std::string Path);

	// Not yet implemented (maybe never). Actually yes. Implemented.
	virtual void resetBuffer();

	// Write the Canvas, Fill with data from generic function. All virtual since we need several different methods
	virtual void populatePixel (const int x, const int y, Function &F);

	// Write data to entire Canvas Buffer
	virtual void populateCanvas (Function &F, const int interval);


	Color getPixel (unsigned int x, unsigned int y);

	// Bmp file generation
	void saveBMP(const char* name);
	unsigned char* createBitmapFileHeader(const int stride);
	unsigned char* createBitmapInfoHeader();
};

#endif
