/* Base class for all kinds of functions. Probably not all members are needed for all kinds of functions. The important
methods are virtual. */

#ifndef FUNCTION_H
#define FUNCTION_H

#include <memory>
#include "Complex.h"

class Function {
private:

	// the argument z
	Complex input;

	// The Complex function value f(z)
	Complex value;

	// This can be used for any sort of iteration of variables
	unsigned int functionIterator = 1;

	// This is used as the maximum number of iterations in sums/iterative functions
	static unsigned int breakoutIterator;

	unsigned int iterations; 

public:

	virtual ~Function () = default;

	// Validating User input (out of domain etc.) obviously depends on the function
	virtual void validateInput ();

	// Access to private members
	unsigned int getFunctionIterator ();
	static unsigned int getBreakoutIterator ();
	unsigned int getIterations ();

	Complex getValue ();
	Complex getInput ();

	// Modifying private members
	void setValue (Complex z);
	void setInput(Complex input);
	void setIterations (unsigned int iter);

	void setFunctionIterator (unsigned int F_I);
	static void setBreakoutIterator (unsigned int M_I);

	// Actually apply the function (obviously this has to be virtual)
	virtual void evaluate ();

	// For use with multithreading
	virtual std::unique_ptr<Function> cloneFunction () const = 0;

	// Returns f(z)
	Complex returnComplex ();

};


#endif