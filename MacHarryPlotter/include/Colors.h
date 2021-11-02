/* Simple Class to deal with colors in colors as 3D objects, also supplies conversion functions for some 
color models (I use HSL for domain coloring (because I can get it very easily from the Complex return value)
but RGB is arguably more convenient for pictures. */

#ifndef COLORS_H
#define COLORS_H

#include "Complex.h"
#include <string>
// #include "Interpolator.h"


// this inheritance is not very logical but simplifies programming
class Color {
private:

	// Three components of color 
	double firstEntry;  // e.g. R
	double secondEntry; // e.g. G
	double thirdEntry;  // e.g. B

	// The current color model (HSL is now default, why? because it is intuitive and close to math)
	const char* colorSpace = "HSL";

public:

	// Default Constructor
	Color ();

	virtual ~Color () = default;

	// Parametrized Constructor
	Color (double param1, double param2, double param3, const char* name);
	
	// Access to private members
	double getFirst ();
	double getSecond ();
	double getThird ();
	std::string getColorSpace ();

	// modify private members
	void setFirst (double param1);
	void setSecond (double param2);
	void setThird (double param3);
	void setColorSpace (const char* name);

	// Good for debugging and nothing else 
	void print ();

	// Domain Coloring (most basic variant)
	void domainColor (Complex z, double gamma);

	// Convert Different Colormodels 
	void HSL_to_RGB ();
	void RGB_to_HSL ();
};

#endif


