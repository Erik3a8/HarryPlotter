/* Class for domain mapping inheriting from the Animation class and thus also from the BaseCanvas class. 
This is what happens when you decide to change the architecture halfway into the project. */

#ifndef STATIC_GRID_H
#define STATIC_GRID_H

#include "Animator.h"
#include "Function.h"
#include "BaseCanvas.h"

class StaticGrid : public Animation {
public:

	/* Initialize and set data all constant. Benchmark exchanging data instead of reallocating every time for varying
	size in case of animations. Definitely observe the deconstructor to avoid memory leaks in said case. */
	StaticGrid (const int x, const int y, const double res, const double CX, const double CY);

	// Amount of pixels between each rendered line 
	static unsigned int lineSpacing;

	// For Outputting files
	static unsigned int fileNumberBMP;
	static unsigned int fileNumberMPG;

	/* I will need a custom deconstructor since I am managing memory manually*/
	~StaticGrid ();

	/* Now functions with the same name actually do the same thing */
	void populatePixel (const int x, const int y, Function &F);
	
	void populateCanvas (Function &F, const int interval);

	// CLI Access Points
	static void configure ();
	static void movie ();
	static void image ();
};

// Default value will be 10


#endif

