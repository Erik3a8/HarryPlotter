#include <math.h>
#include "Mandelbrot.h"


/* Function to plot the Mandelbrot set */

unsigned int Mandelbrot::fractalSelect{0};

void Mandelbrot::setFractalSelect (unsigned int fractalNumber)
{
	fractalSelect = fractalNumber;
}

unsigned int Mandelbrot::getFractalSelect ()
{
	return fractalSelect;
}

void Mandelbrot::evaluate ()
{
	Complex result(0, 0);
	unsigned int iter = 1;

	switch (fractalSelect)
	{
		case 0: 
			for (int i = 0; i < this->getBreakoutIterator(); i++)
			{
				result = result * result + this->getValue();
				if (result.norm2() > 4)
				{
					break;
				}
				iter++;
			}
			break;

		case 1:
			for (int i = 0; i < this->getBreakoutIterator(); i++)
			{
				result = result.conjugate() * result.conjugate() + this->getValue();
				if (result.norm() > 2)
				{
					break;
				}
				iter++;
			}
			break;

		case 2: 
			for (int i = 0; i < this->getBreakoutIterator(); i++)
			{
				result = Complex(abs(result.Re()), abs(result.Im()));
				result = result * result + this->getValue();
				if (result.norm() > 2)
				{
					break;
				}
				iter++;
			} 
			break;

		default: break;
	}

	this->setIterations(iter);
}


std::unique_ptr<Function> Mandelbrot::cloneFunction () const
{
	return std::make_unique<Mandelbrot>(*this);
}





