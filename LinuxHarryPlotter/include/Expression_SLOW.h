#include <stack>
#include <string.h>
#include <vector>

#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "jit_compile.h"
#include "Eisenstein.h"

class Expression {
private: 

	std::stack<int> operators;
	std::stack<Complex> numbers;

	std::vector<int> operatorsArray;
	std::vector<Complex> numbersArray;

	std::stack<Complex> RPN;

	std::string input;

	jit_compile* funcPtr;

public:

	Expression () = default;
	
	jit_compile* getFuncPtr ();
	
	void set (std::string input);

	void prepare ();

	void parse ();

	void reset ();

	Complex compute (Complex z, int N);
};

#endif