/* Easy implementation of Complex numbers for better readability (I am still a mathematician...), 
This class however uses more efficient multiplication/addition for long double. I can basically use 
all the default methods (for double) as this class is really basic. */

#ifndef COMPLEX_H
#define COMPLEX_H

#include <math.h>

class Complex {
private:

	// structure of a complex number
	double realPart;
	double imaginaryPart;

public:
	// access real and imaginary part
	double Re ();
	double Im ();

	// Constructor with values set
	Complex (double re, double im);

	// Empty Constructor (returns 0)
	Complex ();

	// output 
	void print ();

	// overloading operators
	Complex operator+ (Complex z);
	Complex operator- (Complex z);
	Complex operator* (Complex z);
	Complex operator/ (Complex z);


	// norm of a Complex number 
	double norm ();
	double norm2 ();

	Complex conjugate ();

	// Argument of a Complex number
	double argz ();

	// Raise a complex number to an Integer Power
	Complex classPOW (int N);
	Complex classEXP ();

	// Compute the inverse of a Complex number
	Complex inverse ();
	Complex csqrt ();

};


#endif
