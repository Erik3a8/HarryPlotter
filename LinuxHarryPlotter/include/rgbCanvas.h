

#ifndef RGBCANVAS_H
#define RGBCANVAS_H

#include "Colors.h"
#include "BaseCanvas.h"
#include "Animator.h"

class rgbCanvas: public Animation, public Color, public Interpolator {
public:
	// For Outputting files
	static unsigned int fileNumberBMP;
	static unsigned int fileNumberMPG;

	// Maximum and minimum number of iterations taken to escape
	int max, min;

	// Standard Constructor (I could use a default one but meh)
	rgbCanvas (const int x, const int y, const double res, const double CX, const double CY);

	int* escapeTimes = nullptr; 

	// Custom Destructor 
	~rgbCanvas ();

	static Color colormap[6];
	static double points[6]; 


	// interpolating cubic spline based on the colormap 
	void createRGBPolynomialSpline (unsigned int n);

	// evaluating the polynomial created by RGBPolynomialSpline
	void interpolateRGB (unsigned int n, double parameter);

	static void setColorMap (int n, Color colormap[]);

	static void setPoints (int n, double colorPoints[]);

	void firstPass (const int x, const int y, Function &F); 

	void populatePixel (const int x, const int y, Function &F) override;
	
	void populateCanvas (Function &F, const int interval) override;

	// CLI Access Points
	static void configure ();
	static void movie ();
	static void image ();
};


#endif