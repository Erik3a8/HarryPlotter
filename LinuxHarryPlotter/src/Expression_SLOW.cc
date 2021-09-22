#include <string>
#include <stack>
#include <vector>
#include <algorithm>
#include "Support_fast.cc"



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
	int i, j = 0;
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
	return;
}

