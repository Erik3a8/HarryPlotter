/*
It turns out to be beneficial to use precompiled and optimized algorithms for those iterated maps as there is more known
math about them (at least what I know about math) than for generic maps.
*/

#ifndef MANDELBROT_H
#define MANDELBROT_H

#include <memory>
#include "Function.h"

class Mandelbrot : public Function {
private:
	static unsigned int fractalSelect; 

public:

	static unsigned int getFractalSelect ();

	static void setFractalSelect (unsigned int fractalNumber);

	Mandelbrot () = default;

	virtual std::unique_ptr<Function> cloneFunction () const override;

	void evaluate () override;
};


#endif

/* 
Tricorn
Burning ship
Collatz fractals
Multibrot
Lyapunov
*/