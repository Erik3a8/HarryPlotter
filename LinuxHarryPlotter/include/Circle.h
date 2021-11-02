/* Class for representing and drawing circles in the complex plain */

#ifndef CIRCLE_H
#define CIRCLE_H

#include "Complex.h"
#include "Colors.h"
#include "GeoObject.h"


class Circle : public GeoObject {
private:

	static bool filledFlag;

public:

	// Construct circle object with given radius and center
	Circle (double radius, Complex center);

	// Standard Constructor
	Circle () = default;
 
	// Draw the circle on the canvas
	void draw (Color rgb, Color buffer[], unsigned int bufferHeight, unsigned int bufferWidth) override;

	// for use with draw
	void bresenham (int xc, int yc, int x, int y, Color rgb, Color buffer[], unsigned int bufferWidth, unsigned int bufferHeight);

	// Checking whether two circles are the same
	bool operator== (const Circle circ);

	// getters and setters
	double getRadius ();
	double getCurvature ();
	Complex getCenter ();
	static bool getFilledFlag();

	void setRadius (double radius);
	void setCurvature (double curvature);
	void setCenter (Complex center);
	static void setFilledFlag (bool filledFlag);
};

#endif