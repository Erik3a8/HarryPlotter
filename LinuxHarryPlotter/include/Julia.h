/* Function for Julia Fractals, Contains a Quadratic version which is FAST and a version where I can
Iterate a user entered expression (SLOW). Inherits from Expression (Function) class. Note that this class 
is abused as a "normal" function plotter by iterating 0 times. Not pretty. */

#ifndef JULIA_H
#define JULIA_H

#include "Function.h"
#include "Expression_SLOW.h"
// #include "Expression_JIT.h"
#include <memory>


class Julia : public Expression, public Function {
public:

	~Julia () = default;

	// Construct Julia Object
	Julia ();

	// For use with multithreading
	virtual std::unique_ptr<Function> cloneFunction () const override;

	// Compute one Iteration
	void iterate ();

	// Compute the value
	void evaluate ();

	// mostly for debugging
	void print ();

};

#endif



