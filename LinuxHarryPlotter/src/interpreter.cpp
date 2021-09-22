#include <stack>



/* Quick and dirty Complex number parser and RPN calculator Some functions are supported as well. This is quite ugly though. Note that we will 
not check for bad input anymore. This is done by the shunting yard algorithm. */


std::stack<Complex> STACK;


Complex PARSE(double Re, double Im, std::string EXPRESSION)
{
	int i = 0;
	Complex z1, z2, result;
	std::string DIGITS = "";

	while (i < EXPRESSION.length())
	{
		if (CHECK_BRACKET_OPEN(EXPRESSION[i]))
		{	
			// Go to the next character
			i++;

			// Set Real and Imaginary part as 0 by default
			double REAL_PART = 0;
			double IMAGINARY_PART = 0;

			// We will read the digits into this string and then convert it to double
			DIGITS = "";

			// Continue until we close the bracket
			while (not CHECK_BRACKET_CLOSE(EXPRESSION[i])) 
			{	
				// Set the sign variable as one 
				int SGN = 1;
				// do we have negative real part?
				if(EXPRESSION[i] == '-')
				{
					SGN = -1;
					i++;
				}

				// Helper varible to make sure we don't have like two dots in one number
				int DOT_COUNT = 0;
				while (CHECK_NUMBER(EXPRESSION[i]) || CHECK_DOT(EXPRESSION[i]))
				{
					DIGITS += EXPRESSION[i];
					i++;
					DOT_COUNT += CHECK_DOT(EXPRESSION[i]);
					if (DOT_COUNT > 1)
					{
						printf("Error! Invalid Expression");
						exit(EXIT_FAILURE);
					}
				}

				// convert it to double
				REAL_PART = SGN * std::stod(DIGITS);

				// reset helper variables so I can recycle them for the imaginary part
				DIGITS = "";
				SGN = 1;
				DOT_COUNT = 0;

				// If we get a minus set it as minus 1
				if(EXPRESSION[i] == '-')
				{
					SGN = -1;
					i++;
				}
				if(EXPRESSION[i] == '+')
				{
					i++;
				}
				// Same logic as for the real part
				if (CHECK_IMAGINARY_UNIT(EXPRESSION[i]))
				{
					i++;
					while (CHECK_NUMBER(EXPRESSION[i]) || CHECK_DOT(EXPRESSION[i]))
					{
						DIGITS += EXPRESSION[i];
						i++;
						DOT_COUNT += CHECK_DOT(EXPRESSION[i]);
						if (DOT_COUNT > 1)
						{
							printf("Error! Invalid Expression");
							exit(EXIT_FAILURE);
						}
					}
					IMAGINARY_PART = SGN * std::stod(DIGITS);
				}	
			}

			// Set the complex number
			Complex parsed(REAL_PART, IMAGINARY_PART);

			// Push onto Stack
			STACK.push(parsed);
			i++;
		}

		// Entering real numbers as (# + i0) is cumbersome and ugly
		if (CHECK_NUMBER(EXPRESSION[i]))
		{
			DIGITS = "";
			double VALUE;
			int SGN = 1;
			// Negative Number?
			if(EXPRESSION[i] == '-')
			{
				SGN = -1;
				i++;
			}

			int DOT_COUNT = 0;
			while (CHECK_NUMBER(EXPRESSION[i]) || CHECK_DOT(EXPRESSION[i]))
			{
				DIGITS += EXPRESSION[i];
				i++;
				DOT_COUNT += CHECK_DOT(EXPRESSION[i]);
				if (DOT_COUNT > 1)
				{
					printf("Error! Invalid Expression");
					exit(EXIT_FAILURE);
				}
			}
			VALUE = std::stod(DIGITS);
			STACK.push(Complex(VALUE, 0));
		}


		// If we get a "z" it means this is a complex variable
		if (CHECK_VARIABLE(EXPRESSION[i]))
		{
			Complex parsed(Re, Im);
			STACK.push(parsed);
		}

		// Check for Operators
		if (CHECK_OPERATOR(EXPRESSION[i]))
		{
			// Catch Stack underflow
			if (STACK.empty()) 
			{
				printf("Error! Too many Operators entered"); 
				exit(EXIT_FAILURE);
			}

			// Addition
			if (EXPRESSION[i] == '+')
			{	
				z1 = STACK.top();
				STACK.pop();
				z2 = STACK.top();
				STACK.pop();
				result = z1 + z2;
			}

			// Subtraction
			if (EXPRESSION[i] == '-')
			{
				z1 = STACK.top();
				STACK.pop();
				z2 = STACK.top();
				STACK.pop();
				result = z2 - z1;
			}

			// Multiplication 
			if (EXPRESSION[i] == '*')
			{
				z1 = STACK.top();
				STACK.pop();
				z2 = STACK.top();
				STACK.pop();
				result = (z1 * z2);
			}

			// Division
			if (EXPRESSION[i] == '/')
			{
				z1 = STACK.top();
				STACK.pop();
				z2 = STACK.top();
				STACK.pop();
				result = (z2 / z1);
			}
			i++;
			
			// Push result onto stack
			STACK.push(result);
		}

		else 
		{
			// Skip whitespaces
			i++;
		}
	}	
	Complex final_result;
	final_result = STACK.top();
	STACK.pop();
	if (STACK.empty())
	{
		return final_result;
	}
	printf("ERROR! Too many numbers entered!");
	exit(EXIT_FAILURE);	
}