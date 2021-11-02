#include <iostream>
#include <chrono>
#include <algorithm>
#include "Colormaps.h"
#include "rgbCanvas.h"
#include "Mandelbrot.h"
#include "Colors.h"
#include "Interpolator.h"


unsigned int rgbCanvas::fileNumberBMP{0};
unsigned int rgbCanvas::fileNumberMPG{0};

// Default colormap
Color rgbCanvas::colormap[6] = {Color(0, 7, 100, "RGB"), Color(32, 107, 203, "RGB"), Color(237, 255, 255, "RGB"), Color(255, 170, 0, "RGB"), Color(0, 2, 0, "RGB"), Color(0, 0, 0, "RGB")};
double rgbCanvas::points[6] = {0.0, 0.16, 0.42, 0.6425, 0.8575, 1.0}; 


rgbCanvas::rgbCanvas (const int x, const int y, const double res, const double CX, const double CY) 								
{
	setResolution(res);
	this->setWidth(x);
	this->setHeight(y);
	this->realSizeX = this->getWidth() * getResolution();
	this->realSizeY = this->getHeight() * getResolution();
	this->setCenterX(CX);
	this->setCenterY(CY);

	/* Everything goes here. Allocate dynamically so I don't run into problems said he and proceeded
	to manage memory manually instead of using std::vector<double>. Live, Love, Learn */
	this->buffer = new Color[this->getHeight() * this->getWidth()];														
}

rgbCanvas::~rgbCanvas ()
{
    // Release allocated memory
	delete []buffer;
}

void rgbCanvas::setColorMap (int n, const Color colors[])
{
	if (n > MAX_CONTROL_POINTS)
	{
		printf("Too many Colorvalues, cannot set colormap! \n");
		return;
	}

	for (int i = 0; i < n; i++)
	{
		colormap[i] = colors[i];
	}
}

void rgbCanvas::setPoints (int n, const double colorPoints[])
{
	if (n > MAX_CONTROL_POINTS)
	{
		printf("Too many points, cannot set colormap! \n");
		return;
	}

	for (int i = 0; i < n; i++)
	{
		points[i] = colorPoints[i];
	}
}

// Do not call on its own because the polynomial will not be ready
void rgbCanvas::populatePixel (const int x, const int y, Function &F)
{
	double re = (double)(getCenterX() - realSizeX / 2) + x * getResolution();
    double im = (double)(getCenterY() + realSizeY / 2) - y * getResolution();
    
    // Set the Buffer as the value of the function
    F.setValue(Complex(re, im)); 
    F.setInput(Complex(re, im));

    F.evaluate();
    buffer[y * getWidth() + x] = Color(F.getIterations(), 0, 0, "RGB");
}

void rgbCanvas::populateCanvas (Function &F, const int interval)
{
	this->createRGBPolynomialSpline(6);

// Populate the canvas, write the number of iterations to the first color entry
#pragma omp parallel for collapse(2)
    for (int Y = getHeight() - 1; Y >= 0; Y = Y - interval) 
    {
        for (int X = 0; X < getWidth(); X = X + interval) 
        { 
            this->populatePixel (X, Y, *F.cloneFunction());
        }
    }

// find the minimum and maximum entry
    max = 1;
    min = F.getBreakoutIterator();
    for (int Y = getHeight() - 1; Y >= 0; Y = Y - interval)
    {
    	for (int X = 0; X < getWidth(); X = X + interval)
    	{
    		if (this->max < buffer[Y * getWidth() + X].getFirst())
    		{
    			this->max = buffer[Y * getWidth() + X].getFirst();
    		}

    		if (this->min > buffer[Y * getWidth() + X].getFirst())
    		{
    			this->min = buffer[Y * getWidth() + X].getFirst();
    		}
    	}	
    }

    if (max == min)
    {	
    	max = 1000;
    	min = 1;
    	return;
    }

// now retrieve the first color value entry and interpolate the color
    for (int Y = getHeight() - 1; Y >= 0; Y = Y - interval) 
    {
        for (int X = 0; X < getWidth(); X = X + interval) 
        { 
            double parameter = (double)((buffer[Y * getWidth() + X]).getFirst()  - this->min) / (abs(this->max - this->min) + 1);
  			this->interpolateRGB(6, parameter);
  			buffer[Y * getWidth() + X] = *this;
        }
    }
}

void rgbCanvas::createRGBPolynomialSpline (unsigned int n)
{
	if (n > MAX_CONTROL_POINTS)
	{
		printf("Error, too many colorvalues! \n");
		return;
	}

	double redCoeffs[n], greenCoeffs[n], blueCoeffs[n], xValues[n]; 

    // Extract Colormap data
    unsigned int colormapLength = sizeof(colormap) / sizeof(colormap[0]); 
	for (int i = 0; i < colormapLength; i++)
	{
		redCoeffs[i]   = this->colormap[i].getFirst();
		greenCoeffs[i] = this->colormap[i].getSecond();
		blueCoeffs[i]  = this->colormap[i].getThird();
		xValues[i] = this->points[i];
	}

	// Set data of the polynomials 
	this->setyValR(n, redCoeffs);
	this->setyValG(n, greenCoeffs);
	this->setyValB(n, blueCoeffs);
	this->setxVal(n, xValues);

	// Solve the linear equation system to find the spline coefficients
	this->prepareTridiagMatrix (n - 1);
	this->findSplineCoeffs (n - 1);
}

void rgbCanvas::interpolateRGB (unsigned int n, double parameter)
{
	if (parameter < 0 || 1 < parameter)
	{
		printf("Bad parameter, cannot interpolate color! \n");
		return;
	}

	this->hornerEvaluation(n, parameter);
	this->setFirst(getR());
	this->setSecond(getG());
	this->setThird(getB());
}

using namespace std::chrono;

void rgbCanvas::configure ()
{
	char configToken;
	while (configToken != 'b')
	{
		std::cin.ignore();
		printf("CONFIGURE \n\n");
		printf("1: Set width and height in pixels \n");
		printf("2: Set width and height in real size \n");
		printf("3: Set center \n");
		printf("4: Choose fractal \n");
		printf("5: Set colormap \n");
		printf("6: Set maximum number of iterations \n");
		printf("b: Back \n");
		std::cin >> configToken;
		std::cout << "\033[2J\033[1;1H";
		std::cin.clear();

		switch(configToken)
		{
			case '1':
				int userHeight, userWidth;
				printf("Enter desired width in pixels \n");
				std::cin >> userWidth;
				std::cin.clear();
				std::cout << "\033[2J\033[1;1H";

				if (userWidth < 0 || std::cin.fail())
				{
					printf("Bad input, only positive integers allowed! \n\n");
					return;
				}

				setResolution(getResolution() * getWidth() / userWidth);
				setWidth(userWidth);
				printf("Ok! \n");
				printf("Enter desired height in pixels \n");

				std::cin >> userHeight;
				std::cin.clear();
				std::cout << "\033[2J\033[1;1H";
				
				if (userHeight < 0 || std::cin.fail())
				{
					printf("Bad input, only positive integers allowed! \n\n");
					return;
				}
				
				setHeight(userHeight);
				printf("Set width to %d and height to %d \n\n", getWidth(), getHeight());
				break;
				
			case '2': 
				double DuserHeight, DuserWidth;
				printf("Enter desired width \n");

				std::cin >> DuserWidth;
				std::cin.clear();
				std::cout << "\033[2J\033[1;1H";

				if (DuserWidth <= 0 || std::cin.fail())
				{
					printf("Bad input, only positive numbers allowed! \n\n");
					return;
				}

				setResolution(DuserWidth / getWidth());
				printf("Ok! \n");
				printf("Enter desired height \n");

				std::cin >> DuserHeight;
				std::cin.clear();
				std::cout << "\033[2J\033[1;1H";
				
				if (DuserHeight <= 0 || std::cin.fail())
				{
					printf("Bad input, only positive numbers allowed! \n\n");
					return;
				}

				setHeight(DuserHeight / getResolution());
				printf("Set width to %lf and height to %lf \n\n", getWidth() * getResolution(), getHeight() * getResolution());
				break;

			case '3':
				double userCenterX, userCenterY;
				printf("Enter desired real part \n");
				std::cin >> userCenterX;
				std::cin.clear();
				std::cout << "\033[2J\033[1;1H";

				if (std::cin.fail())
				{
					printf("Bad input! \n\n");
					return;
				}

				setCenterX(userCenterX);
				printf("Ok! \n");
				printf("Enter desired imaginary part \n");

				std::cin >> userCenterY;
				std::cin.clear();
				std::cout << "\033[2J\033[1;1H";
				
				if (std::cin.fail())
				{
					printf("Bad input! \n\n");
					return;
				}

				setCenterY(userCenterY);
				printf("Set centerpoint to (%lf, %lf) \n\n", getCenterX(), getCenterY());
				break;	

			case '4':
				printf("Enter desired fractal to be rendered! \n");
				printf("1: Mandelbrot Set \n");
				printf("2: Tricorn \n");
				printf("3: Burning Ship \n");

				char userFractal;

				std::cin >> userFractal;
				std::cin.clear();
				std::cout << "\033[2J\033[1;1H";

				switch(userFractal)
				{
					case '1' : Mandelbrot::setFractalSelect(0);
							   break;

					case '2' : Mandelbrot::setFractalSelect(1);
							   break;

					case '3' : Mandelbrot::setFractalSelect(2);
							   break;

					case 'b' : break;

					default  : printf("Invalid Option! \n");
							   break;
				}
				break;

			case '5': 
				printf("Enter Number of desired colormap \n\n");
				printf("1: Default \n");
				printf("2: Greens \n");
				printf("3: Red-Orange \n");
				printf("4: Blue-Pink-Brown \n");
				printf("b: Back \n");
				char userMap;

				std::cin >> userMap;
				std::cin.clear(); 
				std::cout << "\033[2J\033[1;1H";

	            switch (userMap)
	            {
	            	case '1': setColorMap(6, DEFAULT_COLORS);
	            			  setPoints(6, DEFAULT_POINTS);
	            			  break;

	            	case '2': setColorMap(6, GREENS);
	            			  setPoints(6, GREEN_POINTS);
	            			  break;

	            	case '3': setColorMap(6, RED_ORANGE);
	            			  setPoints(6, RED_ORANGE_POINTS);
	            			  break;

	            	case '4': setColorMap(6, BLUE_PINK_BROWN);
	            			  setPoints(6, BLUE_PINK_BROWN_POINTS);
	            			  break;

	            	case 'b': break;

	            	default : printf("Invalid Option! \n");
	            			  break;
	            }
				
				break;

			case '6': 
				printf("Enter desired number of iterations! \n");
				int iterations;

				std::cin >> iterations;
				std::cin.clear();
				std::cout << "\033[2J\033[1;1H";

				if (iterations <= 0)
				{
					printf("Bad input, must be greater or equal to 0! \n\n");
					break;
				}

				Function::setBreakoutIterator(iterations);
				printf("Maximum number of iterations Set to %d \n\n", Function::getBreakoutIterator());
				break;
				return; 

			case 'b': break;

			default: printf("Invalid Option! \n\n"); break;
		}
	}
}

void rgbCanvas::image ()
{
	char imageToken;
	printf("PLOTTER \n\n");
	printf("This function will output a .bmp file with the following properties: \n");

	// Inform the user about the output that will be created
	std::cout << "Center point :     " << "(" << getCenterX() << ", " << getCenterY() << ")" << std::endl;
	std::cout << "Width in Pixels:   " << getWidth() << std::endl;
	std::cout << "Height in Pixels : " << getHeight() << std::endl;
	std::cout << "Iterations:        " << Function::getBreakoutIterator() << std::endl;

	printf("Continue? y/n \n");

	std::cin >> imageToken;
	std::cout << "\033[2J\033[1;1H"; 

	if(imageToken == 'y')
	{
		std::cin.ignore();
		Mandelbrot fractal;

		rgbCanvas canvas(getWidth(), getHeight(), getResolution(), getCenterX(), getCenterY());

		//Calculate 
		printf("Calculating... \n");

		auto tstart = steady_clock::now();
		canvas.populateCanvas(fractal, 1);
		std::cout << "Calculation finished in: " << (double)duration_cast<microseconds>(steady_clock::now() - tstart).count()/1000000.0 << "s" << std::endl;

		printf("Writing file... \n");

		std::string filename = "EscapeTimeIter_" + std::to_string(fileNumberBMP) + ".bmp";
		const char* c_filename = filename.c_str();
		canvas.saveBMP(c_filename);

		fileNumberBMP++;

		canvas.resetBuffer();

		std::cout << "Success! Saved as: " << filename << "!" << std::endl;
		printf("\n");
	}
	return;
}

