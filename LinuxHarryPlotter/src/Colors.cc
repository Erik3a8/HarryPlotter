/* Implementation of methods for color class. Only thing that's interesting are the conversion methods. */
#include <iostream>
#include <cstdio>
#include <string>
#include <math.h>
#include <cstring>
#include "Complex.h"
#include "Colors.h"





// Default Constructor (Note Default Colormodel is HSL)
Color::Color() {this->colorSpace = "HSL";}

// Parametrized Constructor
Color::Color(double param1, double param2, double param3, const char* name) 
{
	this->firstEntry = param1;
	this->secondEntry = param2;
	this->thirdEntry = param3;
	this->colorSpace = "name";
}

// Setting private members
double Color::getFirst ()
{
	return this->firstEntry;
}

double Color::getSecond ()
{
	return this->secondEntry;
}

double Color::getThird ()
{
	return this->thirdEntry;
}

std::string Color::getColorSpace ()
{
	return this->colorSpace;
}

// Modifying private members
void Color::setFirst (double param1)
{
	this->firstEntry = param1;
}

void Color::setSecond (double param2)
{
	this->secondEntry = param2;
}

void Color::setThird (double param3)
{
	this->thirdEntry = param3;
}

void Color::setColorSpace (const char* name)
{
	this->colorSpace = name;
}

void Color::HSL_to_RGB()
{
	// Check whether we are using the correct color space
	if(strcmp("HSL", this->colorSpace) == true)
	{
		printf("Not using HSL colorspace.");
		exit(EXIT_FAILURE);
	}

	// Renaming so it is easier to read
	double H = this->firstEntry;
	double S = this->secondEntry;
	double L = this->thirdEntry;

	// Check whether we actually use appropriate values (H \in [0, 360], S,L \in [0, 1])
	if(H > 360 || S > 1 || L > 1 || H < 0 || S < 0 || L < 0)
	{
		this->print();
		printf("Bad Colorvalues \n");
		exit(EXIT_FAILURE);
	}

	// Helper Variables
	double C    = (1 - abs(2 * L - 1)) * S;
	int H_60    = floor(H/60);
	double f    = H/60 - floor(H/60);
	double X    = C * (1 - abs(f + H_60 % 2 - 1));
	double M    = L - C/2; 

	// New values
	double R, G, B;

	// Conversion logic it is ugly but what can you do
	switch (H_60) 
	{
		case 0:
			R = C;
			G = X; 
			B = 0;
			break;

		case 1:
			R = X;
			G = C;
			B = 0;
			break;

		case 2:
			R = 0;
			G = C;
			B = X;
			break;

		case 3:
			R = 0;
			G = X;
			B = C;
			break;

		case 4:
			R = X;
			G = 0;
			B = C;
			break;

		case 5:
			R = C;
			G = 0;
			B = X;
			break;

		case 6:
			R = C;
			G = X; 
			B = 0;
			break;
	}

	// Set the new values as actual RGB 0 to 255 and set the Color Model identifier correctly
	firstEntry  = round((R + M) * 255.0);
	secondEntry = round((G + M) * 255.0);
	thirdEntry  = round((B + M) * 255.0);
	colorSpace  = "RGB";
}


// Domain Coloring of complex valued functions
void Color::domainColor (Complex z, double gamma)
{
	// Argument of the result (but converted to degrees)
	double hue = z.argz() * 180/M_PI;

	if (isnan(hue))
	{
		this->firstEntry = 0;
		this->secondEntry = 1;
		this->thirdEntry = 0;
		this->HSL_to_RGB();
		return;
	}

	// Make sure that we only have values between 0 and 360
	if (hue < 0)
	{
		hue += 360;
	}

	// Brightness value
	double L = abs(z.norm()/(1 + z.norm()));

	// Throw into color object
	this->firstEntry = hue;
	this->secondEntry = 1;
	this->thirdEntry = gamma * L;
	this->HSL_to_RGB();
}

// Mainly for debugging
void Color::print () 
{
	std::cout << "(" << this->firstEntry << ", " << this->secondEntry << ", " << this->thirdEntry << ")" << std::endl;
}





