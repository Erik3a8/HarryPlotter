
#ifndef STATIC_COLORDOMAIN_H
#define STATIC_COLORDOMAIN_H

/*
Class for drawing a domain coloring plot of a complex valued function and even modular forms
*/

#include "Animator.h"
#include "Function.h"
#include "BaseCanvas.h"

class StaticColorDomain : public Animation {

public:

	// For Outputting files
	static unsigned int fileNumberBMP;
	static unsigned int fileNumberMPG;

	// In Domain Coloring, to reduce L value
	static double gamma;

	// Standard Constructor (I could use a default one but meh)
	StaticColorDomain (const int x, const int y, const double res, const double CX, const double CY);

	// Custom Destructor 
	~StaticColorDomain ();

	void populatePixel (const int x, const int y, Function &F);
	
	void populateCanvas (Function &F, const int interval);

	// CLI Access Points
	static void configure ();
	static void movie ();
	static void image ();
};



#endif