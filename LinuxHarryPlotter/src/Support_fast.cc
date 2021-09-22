#include <stack>
#include <string>
#include <map>
#include <iostream>
#include <math.h>

/*This will be the place where I put all the ugliness that exists in this projects and dearly hope no one will ever open it
Contains beautiful things such as look-up tables except I don't know the syntax for this. NOTHING TO SEE HERE PEOPLE */


// All of this is for the expression parser. At least this part of the code will look decent. 


// Number of Known Functions
const int NUM_OF_FUNCTIONS = 26;

// Checking for binary Operators
bool checkOperator (char c)
{
	switch(c) 
	{
		case '+' :
		case '-' :
		case '*' :
		case '/' :
		case '^' :
			return true;
		default:
			return false;
	}
}

// Checking for digits
bool checkDigit (char c)
{
	switch (c)
	{
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			return true;
		default:
			return false;
	}
}

bool checkUnaryMinus (char c)
{
	if (c == 'm')
	{
		return true;
	}
	return false;
}

// Checking for binary operators (as values)
bool checkStackOP (int n)
{
	if (n >= 0 && n <= 5)
	{
		return true;
	}
	return false;
}

// Checking for mathematical functions
bool checkFunction (int n)
{
	if (n > 8)
	{
		return true;
	}
	return false;
}

/* Look Up Table for Functions (This might seem paradoxical but i need a modulo 3 property). To add more simply add them here and in the cases
and then implement them in the Complex class. */
std::map<std::string, int> function =
{
	{"-",       0},
	{"*", 	    1},
	{"empty",   2},
	{"+", 	    3},
	{"/", 	    4},
	{"^", 	    5},
	{"(",       6},
	{")",       7},
	{"z",       8},
	{"n",       9},
	
	{"sin",    10},
	{"cos",    11},
	{"tan",    12},
	{"arcsin", 13},
	{"arccos", 14},
	{"arctan", 15},
	{"sinh",   16},
	{"cosh",   17},
	{"tanh",   18},
	{"arsinh", 19},
	{"arcosh", 20},
	{"artanh", 21},
	{"exp",    22},
	{"log",    23},
	{"Eis4",   24},
	{"Eis6",   25},
};

// Check for the dot separator 
int checkDot (char c)
{
	if (c == '.')
	{
		return 1;
	}
	return 0;
}

// Checking for i (needed to parse complex numbers)
bool checkImaginaryUnit (char c)
{
	if (c == 'i') 
	{
		return true;
	}
	return false;
}

// Obviously what it says
bool checkBracketOpen (char c)
{
	if (c == '(')
	{
		return true;
	}
	return false;
}

bool checkBracketClose (char c)
{
	if (c == ')')
	{
		return true;
	}
	return false;
}

// Check for the variable of the function. For now only one variable is supported
bool checkVariable (char c)
{
	if (c == 'z')
	{
		return true;
	}
	return false;
}

// The implementation of the function
void applyFunction (int token, std::stack<Complex> &RPN, Complex z, int n)
{
	Complex num1, num2, result;
	int iterator;
	switch(token)
	{
		// binary Operators
		case 0: 
			num1 = RPN.top();
			RPN.pop();
			num2 = RPN.top();
			RPN.pop();
			result = num2 - num1;
			break;

		case 1:
			num1 = RPN.top();
			RPN.pop();
			num2 = RPN.top();
			RPN.pop();
			result = num1 * num2;
			break;

		case 2: 
			break;

		case 3:
			num1 = RPN.top();
			RPN.pop();
			num2 = RPN.top();
			RPN.pop();
			result = num1 + num2;
			break;

		case 4:
			num1 = RPN.top();
			RPN.pop();
			num2 = RPN.top();
			RPN.pop();
			result = num2 / num1;
			break;

		case 5:
			num1 = RPN.top();
			// std::cout << num1 << std::endl;
			RPN.pop();
			num2 = RPN.top(); 
			// std::cout << num2 << std::endl;
			RPN.pop();
			result = num2.classPOW((int)num1.Re());
			// std::cout << "=P" << result << std::endl;
			break;


		case 8:
			result = z;
			break;

		case 9:
			iterator = n;
			result = Complex(n, 0);
			break;

		// Mathematical Functions
		case 10: 
			num1 = RPN.top();
			RPN.pop();
			result = csin(num1);
			break;

		case 11: 
			num1 = RPN.top();
			RPN.pop();
			result = ccos(num1);
			break;

		case 12: 
			num1 = RPN.top();
			RPN.pop();
			result = ctan(num1);
			break;

		case 13: 
			num1 = RPN.top();
			RPN.pop();
			result = carcsin(num1);
			break;

		case 14:
			num1 = RPN.top();
			RPN.pop();
			result = carccos(num1);
			break;

		case 15:
			num1 = RPN.top();
			RPN.pop();
			result = carctan(num1);
			break;

		case 16:
			num1 = RPN.top();
			RPN.pop();
			result = csinh(num1);
			break;

		case 17:
			num1 = RPN.top();
			RPN.pop();
			result = ccosh(num1);
			break;

		case 18: 
			num1 = RPN.top();
			RPN.pop();
			result = ctanh(num1);
			break;

		case 19: 
			num1 = RPN.top();
			RPN.pop();
			result = carsinh(num1);
			break;

		case 20:
			num1 = RPN.top();
			RPN.pop();
			result = carcosh(num1);
			break;

		case 21:
			num1 = RPN.top();
			RPN.pop();
			result = cartanh(num1);
			break;

		case 22:
			num1 = RPN.top();
			RPN.pop();
			result = cexp(num1);
			break;

		case 23:
			num1 = RPN.top();
			RPN.pop();
			result = clog(num1);
			break;

		case 24: 
			num1 = RPN.top();
			RPN.pop();
			result = Eis(num1, 4);
			break;

		case 25: 
			num1 = RPN.top();
			RPN.pop();
			result = Eis(num1, 6);
			break;
	}
	RPN.push(result);
}



