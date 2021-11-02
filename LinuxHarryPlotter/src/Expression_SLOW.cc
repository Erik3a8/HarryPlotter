#include <string>
#include <stack>
#include <vector>
#include <algorithm>
#include <map>
#include <iostream>
#include <math.h>
#include "Complex.h"
#include "Eisenstein.h"
#include "Expression_SLOW.h"

// Implementation using continued fractions
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

// All of this is for the expression parser. At least this part of the code will look decent. 
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

Complex Eis (Complex z, int weight)
{
	Eisenstein Eis(z, weight);
	Complex result;
	Eis.setBreakoutIterator(200);
	Eis.evaluate();
	result = Eis.getValue();
	return result;
}

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

// This doesn't do anything
jit_compile* Expression::getFuncPtr ()
{
    return this->funcPtr;
}
 

// Implementing a basic mathematical expression parser (it is rather slow), also not safe!
void Expression::set (std::string input)
{
	this->input = input;
}

void Expression::prepare () 
{
	return;
}

void Expression::parse ()
{
	std::stack<Complex> clearNUM;
	std::stack<int> clearOP;
	// define the operator stack (as in the output queue is actually split into an operators and complex stack)
	std::stack<int> temporary;
	// We begin by putting a placeholder on the temporary stack (to avoid immediate underflow)
	temporary.push(2);

	// to check for functions by key
	std::string expression = "";

	// Iterator and helper variable
	int i = 0;
	Complex temp;

	// while there are tokens to be read
	while (i < input.length())
	{

		// Remove all empty operators (value=2) that might have been picked up by the brackets
		if (not temporary.empty())
		{
			if(temporary.top() == 2)
			{
				temporary.pop();
			}
		}

		// Push Number onto number stack and placeholder onto Output stack
		if (checkDigit(input[i]) || checkUnaryMinus(input[i]) || checkImaginaryUnit(input[i]))
		{
			// Convert the string to double and push it onto the numbers queue
			int sgn = 1;
			bool imaginary = false;

			// do we have a negative number?
			if(input[i] == 'm')
			{
				sgn = -1;
				i++;
			}

			if(input[i] == 'i')
			{
				imaginary = true;
				i++;
			}

			// Helper varible to make sure we don't have two dots in one number
			int dotCount = 0;
			
			while (checkDigit(input[i]) || checkDot(input[i]))
			{
				expression += input[i];
				i++;
				dotCount += checkDot(input[i]);
				if (dotCount > 1)
				{
					printf("Error! Invalid Expression \n");
					operators = clearOP;
					numbers = clearNUM;
					return;
				}
			}

			if (expression == "")
			{	
				if (imaginary == true)
				{
					temp = Complex(0, 1);
				}

				else 
				{
					operators = clearOP;
					numbers = clearNUM;
					return;
				}
			}

			// convert it to Complex
			if (imaginary == true && expression != "")
			{
				temp = Complex(0, sgn * std::stod(expression));
			}

			if (imaginary == false && expression != "")
			{
				temp = Complex(sgn * std::stod(expression), 0);
			}
			

			// reset helper variables 
			sgn = 1;
			dotCount = 0;
			expression = ""; 

			// Push to Numbers stack
			numbers.push(temp);

			// push a placeholder for a number onto the operator stack
			operators.push(2);
			// printf("Number Pushed to OPERATORS! \n");
		}

		// Read a variable (only z is supported as variable key)
		if (checkVariable(input[i]))
		{
			operators.push(function[input.substr(i, 1)]);
			// printf("Variable Pushed to OPERATORS! \n");
			i++;
		}
		
		
		// If we find a operator there is a terrible condition
		if (checkOperator(input[i]))
		{
			// There are still operators on the temporary stack
			while (not temporary.empty() 
				// We are actually looking at a binary operator and not a function
				&& checkStackOP (temporary.top()) 
				// Checking Precedence and Associativity
		      	&& ((temporary.top() % 3 > function[input.substr(i, 1)] % 3))
		      	// Checking this is not an opening bracket 
			   	&& temporary.top() != 6)
			{	
				// Push all Operators onto output operator stack
				int op = temporary.top();
				temporary.pop();
				operators.push(op);
				// printf("Operator Pushed to OPERATORS in while! \n");
			}
			
			// Now push the operator onto the temporary stack
			temporary.push(function[input.substr(i, 1)]);
			// printf("Pushed Operator to TEMP! \n");
		}

		// Read a string which might be a function
		if (not checkDigit(input[i]) && not checkOperator(input[i]) 
									   && i < input.length() 
									   && input[i] != ' ' 
									   && not checkBracketOpen(input[i]) 
									   && not checkBracketClose(input[i]))
		{
			while (not checkDigit(input[i]) && not checkOperator(input[i]) 
										  	  && i < input.length() 
										      && input[i] != ' ' 
										      && not checkBracketOpen(input[i]) 
										      && not checkBracketClose(input[i]))
			{
				// Build the string
				expression += input.substr(i, 1);
				i++;
			}

			// if we find a function with that name push it onto the temporary operator stack
			if (function.contains(expression) and expression != "")
			{
				temporary.push(function[expression]);
				expression = "";
				// printf("Pushed Math function to TEMP\n");
			}

			// If we don't find the function reset the stacks and exit
			else 
			{
				operators = clearOP;
				numbers = clearNUM;

				printf("Unknown Function \n");
				return;
			}
		}

		if (checkBracketOpen(input[i]))
		{
			// Push the opening bracket onto the temporary stack
			temporary.push(function[input.substr(i, 1)]);
			// printf("Pushed '(' to TEMP! \n");
		}

		if (checkBracketClose(input[i]))
		{
			// as long as there isn't another opening bracket push operators onto the output operator stack
			if (not temporary.empty())
			{
				while (temporary.top() != function["("])
				{
					int op = temporary.top();
					temporary.pop();
					operators.push(op);
					// printf("Operator Pushed while not '('! \n");
				}
			}

			// If now the Stack has run out and we don't see an opening Parentheses something went wrong
			if(temporary.empty() || temporary.top() != function["("])
			{
				operators = clearOP;
				numbers = clearNUM;
				printf("Mismatched Parentheses! \n");
				return;
			}

			// If we find the opening bracket remove it and check whether there is a mathematical function left
			if (temporary.top() == function["("])
			{
				temporary.pop();
				while (not temporary.empty())
				{
					if (checkFunction(temporary.top()))
					{
						int func = temporary.top();
						temporary.pop();
						operators.push(func);
					}

					else 
					{
						break;
					}
				}
			}
		}

		// Skip Whitespaces
		i++;
	}

	// Push the remaining operators onto the output operator stack
	while (not temporary.empty())
	{
		// Catch mismatched Parentheses
		if (temporary.top() == function["("])
		{
			operators = clearOP;
			numbers = clearNUM;
			printf("Mismatched Parentheses! \n");
			return;
		}

		// discard any empty operators from the temporary stack
		int op = temporary.top();
		if(op != 2)
		{
			temporary.pop();
			operators.push(op);
			// std::cout << "Pushed Operator: " << OP << std::endl;
		
		}
	}
	// reverse both stacks
	int n = 0;

	while (not operators.empty())
	{
		int op = operators.top();
		operators.pop();
		operatorsArray.push_back(op); 
		n++;
	}

	std::reverse(operatorsArray.begin(), operatorsArray.end());
	
	int m = 0;

	while (not numbers.empty())
	{
		Complex num = numbers.top();
		numbers.pop();
		numbersArray.push_back(num);
		m++;
	}

	std::reverse(numbersArray.begin(), numbersArray.end());

	// done
	printf("Parsed Successfully! \n");
	std::cout << "Expression Entered: " << input << std::endl;
}

Complex Expression::compute (Complex z, int N)
{
	Complex temp;
	int op;
	int i = 0, j = 0;
	// There are more operators than numbers
	while (i < operatorsArray.size())
	{
		while (j < numbersArray.size())
		{
			// Get the operator
			op = operatorsArray[i];
			// std::cout << "Operator: " << OP << std::endl;

			// If it is empty add a number, if it is not empty then evaluate.
			if (op != 2)
			{
				applyFunction(op, RPN, z, 0);
			}
			else 
			{
				temp = numbersArray[j];
				RPN.push(temp);
				j++;
			}
			i++;
		}

		if (i == operatorsArray.size())
		{
			break;
		}

		// Pop all remaining operators and evaluate 
		op = operatorsArray[i];
		if (op != 2)
		{
			applyFunction(op, RPN, z, 0);
		}
		i++;
	}
	// done
	Complex finalResult = RPN.top();
	
	// FINAL_RESULT.PRINT();
	return finalResult;
}

void Expression::reset()
{
	this->input = "";
	return;
}

