#include <cstdio>
#include <math.h>



Eisenstein::Eisenstein (Complex input, const unsigned int weight)
{
	input = Complex(input.Re() - floor(input.Re()), input.Im()); 
	this->setInput(input);
	this->weight = weight;
	this->validateInput();
}

Eisenstein::Eisenstein (const unsigned int W)
{
	this->setInput(Complex(0, 0));
	this->weight = weight;
	this->validateInput();
}

void Eisenstein::validateInput ()
{
	if (weight < 0)
	{
		printf("Invalid weight, cannot be less than 0! \n");
		exit(EXIT_FAILURE);
	}
	if (weight % 2 != 0)
	{
		printf("Invalid weight, cannot be uneven number! \n");
		exit(EXIT_FAILURE);
	}
	if (this->getInput().Im() < 0)
	{
		printf("Imaginary Part out of Bounds. Eisenstein Series are only defined on the upper half plane. \n");
		exit(EXIT_FAILURE);
	}
}

int Eisenstein::getWeight ()
{
	return this->weight;
}

std::unique_ptr<Function> Eisenstein::cloneFunction () const
{
	return std::make_unique<Eisenstein>(*this);
}

// Complex Eisenstein::ITERATE (int c, int d)
// {
// 	// The Complex Number in the Upper Half Plane that is my actual argument
// 	Complex tau = this->GET_input();

// 	/* Actually those are just whole numbers but it is easier to read that way and I can use 
// 	my performance tweaked complex numbers class */ 

// 	Complex alpha (c, 0);
// 	Complex beta (d, 0);
// 	Complex zero;

// 	// Any Point that is neither on the x nor the y axis
// 	if (c != 0 && d != 0) 
// 	{	
// 		return ((alpha * tau + beta).POW(WEIGHT)).INVERSE() + ((alpha * tau - beta).POW(WEIGHT)).INVERSE();
// 	}

// 	// Points on the y or x axis but without zero
// 	if (d == 0 ^ c == 0)
// 	{
// 		return (((alpha * tau + beta).INVERSE()).POW(WEIGHT));
// 	}

// 	return zero;
// }

// Evaluate using lattice sums (slow, but produces a nice picture)
// void Eisenstein::EVALUATE () 
// {
// 	this->SET_value(Complex (0,0));
// 	// Sum along the radius and sum the points in the lattice that lie within that radius
// 	for (int c = 0; c <= this->GET_ITERATOR(); c++) 
// 	{	
// 		for (int d = 0; sqrt (c * c + d * d) <= this->GET_ITERATOR(); d++) 
// 		{	
// 			if (not (c == 0 && d == 0))
// 			{
// 				this->SET_value(this->ITERATE(c, d) + this->GET_value());
// 			}
			
// 		}
// 	}
// }

// int DIVISOR_SIGMA (int N, int k)
// {
// 	int result = 0;
// 	for (int i = 1; i <= sqrt(N); i++)
//     {
//         if (N % i == 0)
//         {
            
//             if (N / i == i)
//             {
//                 result += pow(i, k);
//             }
 
//             else
//             {
//                 result += pow(i, k) + pow(N / i, k);
//             }
//         }
//     }
//     return result;
// }


// Evaluate using theta functions (fast, but has some weird artifacts close to the real axis)
void Eisenstein::evaluate ()
{
	Complex q = (this->getInput() * Complex(2 * M_PI, 0) * Complex(0, 1)).classEXP();
	
	this->setValue(Complex(0,0));

	if (this->getInput().Im() < 0.1)
	{
		this->setInput(Complex(-1, 0) * this->getInput().inverse());

		Complex q = (this->getInput() * Complex(2 * M_PI, 0) * Complex(0, 1)).classEXP();
		Complex p = q;

		for (int i = 1; i < this->getBreakoutIterator(); i++)
		{
			this->setValue(((Complex(i, 0).classPOW(weight - 1) * q) / (Complex(1, 0) - q)) + this->getValue());
			q = p * q;
		}
		this->setValue ((this->getInput()).classPOW(weight) * (Complex(1, 0) + Complex(CONSTANTS[weight / 2 - 2], 0) * this->getValue()));
	}

	else 
	{
		Complex p = q;
		for (int i = 1; i < this->getBreakoutIterator(); i++)
		{
			this->setValue(((Complex(i, 0).classPOW(weight - 1) * q) / (Complex(1, 0) - q)) + this->getValue());
			q = p * q;
		}
		this->setValue (Complex(1, 0) + Complex(this->CONSTANTS[weight / 2 - 2], 0) * this->getValue());
	}
}

// Evaluate using the sigma divisor sum (very slow)
// void Eisenstein::EVALUATE ()
// {
// 	Complex q = cexp(this->GET_input() * Complex(2 * M_PI, 0) * Complex(0, 1));
// 	Complex p = q;
// 	this->SET_value(Complex(0,0));

// 	for (int i = 1; i < 400; i++)
// 	{
// 		this->SET_value(Complex(DIVISOR_SIGMA(i, 3), 0) * q + this->GET_value());
// 		q = p * q;
// 	}

// 	this->SET_value (Complex(1, 0) + Complex(240, 0) * this->GET_value());
// }

Complex Eis (Complex z, int weight)
{
	Eisenstein Eis(z, weight);
	Complex result;
	Eis.setBreakoutIterator(200);
	Eis.evaluate();
	result = Eis.getValue();
	return result;
}

	