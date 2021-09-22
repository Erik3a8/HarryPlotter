
#ifndef EISENSTEIN_H
#define EISENSTEIN_H

#include "Complex.h"
#include "Function.h"


class Eisenstein : public Function {
private:

	unsigned int weight;
	static const unsigned int MAX_WEIGHT = 32;
	static const constexpr double CONSTANTS[MAX_WEIGHT / 2] = 

	{240, -504, 480, -264, 65520/691, -24, 16320/3617, -28728/43867, 13200/174611, -552/77683, 131040/236364091, -24/657931,
	 6960/3392780147, -171864/1723168255201, 32640/7709321041217, 24/151628697551};


public:

	void validateInput() override;

	~Eisenstein () = default;

	// Eisenstein Series object 
	Eisenstein (Complex input, const unsigned int weight);
	Eisenstein (const unsigned int weight);

	// Access to private members
	int getWeight ();

	virtual std::unique_ptr<Function> cloneFunction () const override;

	// Modifying private members
	void setIteration (int N);

	Complex iterate(int c, int d);
		
	void evaluate() override;
};

#endif