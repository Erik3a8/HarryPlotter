/* Class for Geometry (for now only a apollonian gaskets) inheriting from the Animation class and thus also from the BaseCanvas class. 
This is what happens when you decide to change the architecture halfway into the project. */

#ifndef GEOMETRY_CANVAS_H
#define GEOMETRY_CANVAS_H

#include "Colormaps.h"
#include "Animator.h"
#include "Function.h"
#include "BaseCanvas.h"
#include "GeoObject.h"
#include "Construction.h"

class GeometryCanvas : public Animation {
private:

	static unsigned int numberOfColors;

public:

	/* Initialize and set data all constant. Benchmark exchanging data instead of reallocating every time for varying
	size in case of animations. Definitely observe the deconstructor to avoid memory leaks in said case. */
	GeometryCanvas (const int x, const int y, const double res, const double CX, const double CY);

	// For Outputting files
	static unsigned int fileNumberBMP;
	static unsigned int fileNumberMPG;

	/* I will need a custom deconstructor since I am managing memory manually*/
	~GeometryCanvas ();

	void populateGeoCanvas (Construction constr);

	static void setNumberOfColors (unsigned int colors);

	static unsigned int getNumberOfColors ();

	// CLI Access Points
	static void configure ();
	static void movie ();
	static void image ();
};



#endif