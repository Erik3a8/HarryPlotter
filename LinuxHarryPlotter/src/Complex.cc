#include <iostream>
#include <math.h>
#include "Complex.h"

#ifndef COMPLEX_IMP
#define COMPLEX_IMP

double Complex::Re () 
{
	return realPart;
}

double Complex::Im () 
{
	return imaginaryPart;
}

// Constructor (with given Real and Imaginary part)
Complex::Complex (double re, double im) 
{
	realPart = re;
	imaginaryPart = im;
}

Complex::Complex ()
{
	realPart = 0;
	imaginaryPart = 0;
}

// Print Complex numbers in the canonical way... This turns out to be rather annoying
void Complex::print () 
{
	if (this->imaginaryPart > 0) {
		std::cout << this->realPart
				  << " + " 
				  << "i"
				  << this->imaginaryPart;
	}
	if (this->imaginaryPart == 0) {
		std::cout << this->realPart;
	}

	// No second minus sign required
	if (this->imaginaryPart < 0) {
		std::cout << this->realPart 
				  << this->imaginaryPart
				  << "i"; 
				  
	}
}

// usual rules for computation in $\\C$ (except now with SSE instructions)

Complex Complex::operator+ (Complex z) 
{
	return Complex(this->Re() + z.Re(), this->Im() + z.Im());
}

Complex Complex::operator- (Complex z) 
{
	return Complex(this->Re() - z.Re(), this->Im() - z.Im());
}

// Multiplication this way is a tiny bit faster although also a bit more prone to rounding errors
Complex Complex::operator* (Complex z) 
{	
  	return Complex(this->Re() * z.Re() - this->Im() * z.Im(), this->Im() * z.Re() + this->Re() * z.Im());
}

Complex Complex::operator* (double lambda)
{
	return Complex(this->Re() * lambda, this->Im() * lambda);
}

Complex Complex::operator/ (Complex z) 
{	
	double denominator = z.norm2();
	return Complex ((Re() * z.Re() + Im() * z.Im()) / denominator, (Im() * z.Re() - Re() * z.Im()) / denominator);
}

bool Complex::operator== (Complex z)
{
	if (this->Re() == z.Re() && this->Im() == z.Im())
	{
		return true;
	}
	return false;
}

// return the euclidean norm of a complex number using Pythagoras' Theorem.
double Complex::norm () 
{
	return sqrt (Re () * Re () + Im () * Im ());
}

double Complex::norm2 ()
{
	return abs (Re() * Re() + Im () * Im ());
}

double Complex::argz ()
{
	return atan2 (this->Im(), this->Re());
}

// I will trust the standard definitions of math.h here
Complex Complex::classEXP()
{
	return Complex(exp(this->Re())*cos(this->Im()), exp(this->Re())*sin(this->Im()));
}

Complex Complex::classPOW(int N)
{
	Complex temp = Complex(1, 0);
	if (N < 0)
	{
		this->inverse();
		N = -N;
	}
	if (N == 0)
	{
		return Complex(1, 0);
	}
	if (N == 1)
	{
		return *this;
	}

	while (N > 1)
	{
		if (N % 2 == 0)
		{
			*this = *this * *this;
			N = N / 2;
		}

		else 
		{
			temp = *this * temp;
			*this = *this * *this;
			N = (N - 1) / 2;
		}
	}

	return *this * temp; 
}


// Implement error catching of division by zero
Complex Complex::inverse () 
{
	double denom = this->norm2();
	return Complex (this->Re () / denom, - this->Im () / denom);
}

Complex Complex::csqrt ()
{
	double l = this->norm();
	double u = sqrt((l + this->Re()) / 2);
	double v = (1 - signbit(this->Im()) * 2) * sqrt((l - this->Re()) / 2);
	
	return Complex(u, v);
}

Complex Complex::conjugate ()
{
	return Complex(this->Re(), (-1) * this->Im());
}

// Add new functions here. Just preface the name with a "c". The expression class will handle everything else for you (if you are on mac).

// Implementation using standard libraries and eulers identity
// Complex cexp(Complex z)
// {
// 	double r = exp(z.Re());
// 	return Complex(r * cos(z.Im()), r * sin(z.Im()));
// }






#endif













