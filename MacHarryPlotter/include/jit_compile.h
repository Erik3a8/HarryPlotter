
#include "Complex.h"
#include <string.h>
#include <cstdio>
#include <iostream>

class jit_compile {
protected:
	std::string INPUT;
public:
	virtual Complex FUNCTION (Complex z, int N) const = 0;
	void INIT (std::string USER_INPUT) {INPUT = USER_INPUT;}
	virtual ~jit_compile () {};
};