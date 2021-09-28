#include <iostream>
#include <pmmintrin.h>
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
	__m128d vec1 = _mm_set_pd(this->Im(), this->Re());
	__m128d vec2 = _mm_set_pd(z.Im(), z.Re());
	__m128d res = _mm_add_pd(vec1, vec2);

	Complex* result = (Complex* )&res;

	return *result;
}

Complex Complex::operator- (Complex z) 
{
	__m128d vec1 = _mm_set_pd(this->Im(), this->Re());
	__m128d vec2 = _mm_set_pd(-z.Im(), -z.Re());
	__m128d res = _mm_add_pd(vec1, vec2);

	Complex* result = (Complex* )&res;

	return *result;
}

// Multiplication this way is a tiny bit faster although also a bit more prone to rounding errors
Complex Complex::operator* (Complex z) 
{	
	double real = this->Re();
	double imaginary = this->Im();

	__m128d num1 = _mm_loaddup_pd(&real);
	__m128d num2 = _mm_set_pd(z.Im(), z.Re());
	__m128d num3 = _mm_mul_pd(num2, num1);

	num1 = _mm_loaddup_pd(&imaginary);
	num2 = _mm_shuffle_pd(num2, num2, 1);
	num2 = _mm_mul_pd(num2, num1);

	num3 = _mm_addsub_pd(num3, num2);
  
  	Complex* result = (Complex* )&num3;

  	return *result;
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

Complex POW (Complex z, int N) 
{
	Complex temp = Complex(1, 0);
	if (N < 0)
	{
		z.inverse();
		N = -N;
	}
	if (N == 0)
	{
		return Complex(1, 0);
	}
	if (N == 1)
	{
		return z;
	}

	while (N > 1)
	{
		if (N % 2 == 0)
		{
			z = z * z;
			N = N / 2;
		}

		else 
		{
			temp = z * temp;
			z = z * z;
			N = (N - 1) / 2;
		}
	}

	return z * temp; 
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

// Implementation using continued fractions
Complex cexp(Complex z)
{
	// the modified lentz algorithm to evaluate the continued fraction described by Khovansky
	Complex fredu, Credu, Dredu, fnorm, Cnorm, Dnorm;
	Complex tiny(1e-30, 0);
	Complex zero(0, 0);

	// Range reduction basically exp(z) = exp(z / |z|^2 )^(floor(|z|^2)) * exp(z / |z|^2 * (|z|^2 - floor(|z|^2)))
	Complex zRedu = z / Complex(z.norm2(), 0);

	// Coefficients of the continued fraction
	Complex A(6, 0);

	// |z|^2 - floor(|z|^2)
	Complex zNorm(z.norm2() - floor(z.norm2()), 0);

	// floor(|z|^2)
	int normFactor = floor(z.norm2());

	// squares for faster calculation
	Complex zRedu2 = POW(zRedu, 2);
	Complex zNorm2 = POW(zRedu * zNorm, 2);  

	// helper variables for lentz algorithm
	Cnorm = Credu = Complex(1, 0);
	Dnorm = Dredu = Complex(0, 0);
	fnorm = fredu = Complex(1, 0);

	// First terms of the fraction
	Credu = Complex(2, 0) - zRedu + Complex(2, 0) * zRedu / Credu;
	Cnorm = Complex(2, 0) - (zRedu * zNorm) + Complex(2, 0) * (zRedu * zNorm) / Cnorm;

	if (Credu == zero)
	{
		Credu = tiny;
	}

	if (Cnorm == zero)
	{
		Cnorm = tiny;
	}

	Dredu = ((Complex(2, 0) - zRedu) + Complex(2, 0) * zRedu * Dredu);
	Dnorm = Complex(2, 0) - (zNorm * zRedu) + Complex(2, 0) * (zNorm * zRedu) * Dnorm;

	if (Dnorm == zero)
	{
		Dnorm = tiny;
	}

	if (Dredu == zero)
	{
		Dredu = tiny;
	}

	Dredu = Dredu.inverse();
	Dnorm = Dnorm.inverse();

	fredu = fredu * Credu * Dredu;
	fnorm = fnorm * Cnorm * Dnorm;

	// maximum of thousand iterations should be enough (usually converges after 5 iterations)
	for (int i = 0; i < 1000; i++)
	{
		Credu = A + zRedu2 / Credu;
		Cnorm = A + zNorm2 / Cnorm;
		
		if (Credu == zero)
		{
			Credu = tiny;
		}

		if (Cnorm == zero)
		{
			Cnorm = tiny;
		}

		Dredu = (A + zRedu2 * Dredu);
		Dnorm = (A + zNorm2 * Dnorm);

		if (Dredu == zero)
		{
			Dredu = tiny; 
		}

		if (Dnorm == zero)
		{
			Dnorm = tiny;
		}

		Dredu = Dredu.inverse();
		Dnorm = Dnorm.inverse();

		fredu = fredu * Credu * Dredu;
		fnorm = fnorm * Cnorm * Dnorm;

		A = Complex(4 + A.Re(), 0);

		// Condition for breaking the loop
		if ((Credu * Dredu - Complex(1, 0)).norm2() < 1e-30 && (Cnorm * Dnorm - Complex(1, 0)).norm2() < 1e-30)
		{
			break;
		}
	}

	// putting it all together 
	return POW(fredu, normFactor) * fnorm;
}


Complex csin(Complex z)
{
	Complex exp_part = cexp(z * Complex(0, 1));
	return Complex(0, -0.5) * (exp_part - exp_part.inverse());
}

Complex ccos(Complex z)
{
	Complex exp_part = cexp(z * Complex(0, 1));
	return Complex(0.5, 0) * (exp_part + exp_part.inverse());
}

Complex ctan(Complex z)
{
	return csin(z) / ccos(z);
}

Complex ccosh(Complex z)
{
	return ccos(Complex(0, 1) * z);
}

Complex csinh(Complex z)
{
	return Complex(0, -1) * csin(Complex(0, 1) * z);
}

Complex ctanh(Complex z)
{
	return csinh(z) / ccosh (z);
}

Complex clog(Complex z)
{
	return Complex(log(z.norm()), z.argz());
}

Complex carcsin(Complex z)
{
	return Complex(0, -1) * clog(Complex(0, 1) * z + (Complex(1, 0) - z * z).csqrt());
}

Complex carccos(Complex z)
{
	return Complex(0, -1) * clog(z + (Complex(-1, 0)  + z * z).csqrt());
}
 
Complex carctan(Complex z)
{
	return Complex(0, 0.5) * (clog(Complex(1, 0) - Complex(0, 1) * z) - clog(Complex(1, 0) + Complex(0, 1) * z));
}

Complex carsinh(Complex z)
{
	return clog(z + (z * z + Complex(1, 0)).csqrt());
}

Complex carcosh(Complex z)
{
	return clog(z + (z + Complex(1, 0)).csqrt() * (z - Complex(1, 0)).csqrt());
}

Complex cartanh(Complex z)
{
	return Complex(0.5, 0) * (clog(Complex(1, 0) + z) - clog(Complex(1, 0) - z));
}



#endif













