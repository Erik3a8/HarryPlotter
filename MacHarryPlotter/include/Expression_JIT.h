#ifndef EXPRESSION_H
#define EXPRESSION_H


#include "jit_compile.h"
#include "Eisenstein.h"

class Expression {
private:
	// The string with the math function
	std::string userInput;

	// 
	std::string last;

	// Where the temporary files are stored
	std::string OUTPATH = "/tmp";

	// Pointer to compiled function object
	jit_compile* FuncPtr;

	// Dynamic Library 
	void* LOAD_DYNAMIC_LIB = nullptr;

public:
	// Standard Constructor
	Expression () = default;

	// Standard Destructor
	virtual ~Expression () = default;

	// Set the string to be parsed
	void set (std::string userInput);

	// translate it to C++ code
	void prepare ();

	// Return the Function pointer
	jit_compile* getFuncPtr ();

	// The JIT-Compiled program (Function Pointer to a jit_compile class object)
	std::string emitCode ();	

	// invoking the compiler at runtime
	void parse ();

	// evaluating the function pointer
	Complex compute (Complex z, int N);

	// Unlinking the library and resetting input
	void reset ();

};

#endif
