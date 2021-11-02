#include <iostream>
#include <chrono>
#include "StaticColorDomain.h"
#include "Julia.h"

unsigned int StaticColorDomain::fileNumberBMP{0};
unsigned int StaticColorDomain::fileNumberMPG{0};
double StaticColorDomain::gamma{0.85};

StaticColorDomain::StaticColorDomain (const int x, const int y, const double res, const double CX, const double CY) 								
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

StaticColorDomain::~StaticColorDomain ()
{
    // Release allocated memory
	delete []buffer;
}

void StaticColorDomain::populatePixel (const int x, const int y, Function &F)
{
	double re = (double)(getCenterX() - realSizeX / 2) + x * getResolution();
    double im = (double)(getCenterY() + realSizeY / 2) - y * getResolution();
    
    // Set the buffer as the value of the function
    F.setValue(Complex(re, im)); 
    F.setInput(Complex(re, im));
    
    // Evaluate the function
  	Complex z = F.returnComplex();

  	Color Pixel;
  	Pixel.domainColor (z, gamma);
    buffer[y * getWidth() + x] = Pixel;
}

void StaticColorDomain::populateCanvas (Function &F, const int interval)
{
#pragma omp parallel for collapse(2)
    for (int y = getHeight() - 1; y >= 0; y = y - interval) 
    {
        for (int x = 0; x < getWidth(); x = x + interval) 
        { 
            this->populatePixel(x, y, *F.cloneFunction());
        }
    }
}

using namespace std::chrono;

// So begins the enormous command line interface...
void StaticColorDomain::configure ()
{
	char configToken;
	while (configToken != 'b')
	{
		std::cin.ignore();
		printf("CONFIGURE \n\n");
		printf("1: Set width and height in pixels \n");
		printf("2: Set width and height in real size \n");
		printf("3: Set center \n");
		printf("4: Set number of iterations (julia) \n");
		printf("5: Set Gamma value \n");
		printf("6: Set framerate \n");
		printf("7: Set length in seconds \n");
		printf("8: Toggle breakout iteration for rendering \n");
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

			case '5': 
				printf("Enter desired Gamma value! \n");
				double userGamma;

				std::cin >> userGamma;
				std::cin.clear(); 
				std::cout << "\033[2J\033[1;1H";

				if (userGamma < 0 || userGamma > 1 || std::cin.fail())
				{
					printf("Bad input, must be between 0 and 1! \n");
					return;
				}
				gamma = userGamma;
				printf("Set Gamma to %lf \n\n", gamma);
				break;

			case '6': 
				printf("Enter desired framerate (Only 24, 25, 30, 50, or 60)! \n");
				int userFps;

				std::cin >> userFps;
				std::cin.clear();		
				std::cout << "\033[2J\033[1;1H";	

				if (userFps == 24 || userFps == 25 || userFps == 30 || userFps == 50 || userFps == 60)
				{
					// Update the fps and also the number of frames (FPS should not alter the length of the video)
					setFrameCount(getFrameCount() * userFps / getFps());
					setFps(userFps);
					printf("Set framerate to %d \n\n", getFps());
					break;
				}
				printf("Bad input \n\n");
				return;

			case '7':
				printf("Enter desired length in seconds! \n");
				double userLength;

				std::cin >> userLength;
				std::cin.clear();
				std::cout << "\033[2J\033[1;1H";

				if (userLength <= 0)
				{
					printf("Bad input, must be a positive number! \n\n");
					return;
				}

				setFrameCount(userLength * getFps());
				printf("Set length to %d frames \n\n", getFrameCount()); 
				break;

			case '8':
				printf("Increase number of iterations with each frame y/n? \n");

				char toggle;
				std::cin >> toggle;
				std::cin.clear();
				std::cout << "\033[2J\033[1;1H";

				switch(toggle)
				{
					case 'y': 
						setBreakoutFlag(true);
						printf("Increasing iterations by 1 with each frame! \n");
						break;

					case 'n':
						setBreakoutFlag(false);
						printf("Constant number of iterations! \n");
						break;

					default: 
						printf("Invalid Option! \n");
						break;
				}
				break;	


			case 'b': break;

			default: printf("Invalid Option! \n\n"); break;
		}
	}
}

void StaticColorDomain::image ()
{
	char imageToken;
	printf("PLOTTER \n\n");
	printf("This function will output a .bmp file with the following properties: \n");

	// Inform the user about the output that will be created
	std::cout << "Center point :     " << "(" << getCenterX() << ", " << getCenterY() << ")" << std::endl;
	std::cout << "Width in Pixels:   " << getWidth() << std::endl;
	std::cout << "Height in Pixels : " << getHeight() << std::endl;
	std::cout << "Iterations:        " << Function::getBreakoutIterator() << std::endl;
	std::cout << "Gamma value:       " << gamma << std::endl;

	printf("Continue? y/n \n");

	std::cin >> imageToken;
	std::cout << "\033[2J\033[1;1H"; 

	if(imageToken == 'y')
	{
		std::cin.ignore();
		Julia function;
		std::string userInput;

		StaticColorDomain canvas(getWidth(), getHeight(), getResolution(), getCenterX(), getCenterY());

		printf("Enter Expression! \n");
		std::getline(std::cin, userInput);

		if (std::cin.fail())
		{
			printf("Error Reading input! \n");
			return;
		}

		//Prepare the Function
		function.set(userInput);
		function.prepare();
		function.parse();

		//Calculate 
		printf("Calculating... \n");

		auto tstart = steady_clock::now();
		canvas.populateCanvas(function, 1);
		std::cout << "Calculation finished in: " << (double)duration_cast<microseconds>(steady_clock::now() - tstart).count()/1000000.0 << "s" << std::endl;

		printf("Writing file... \n");

		std::string filename = "ColorDomain_" + std::to_string(fileNumberBMP) + ".bmp";
		const char* c_filename = filename.c_str();
		canvas.saveBMP(c_filename);

		fileNumberBMP++;

		function.reset();
		canvas.resetBuffer();

		std::cout << "Success! Saved as: " << filename << "!" << std::endl;
		printf("\n");
	}
	return;
}

void StaticColorDomain::movie ()
{
	char videoToken;
	printf("ANIMATOR \n\n");
	printf("This function will output a .mpg file with the following properties: \n");

	// Inform the user about the output that will be created
	std::cout << "Center point :     " << "(" << getCenterX() << ", " << getCenterY() << ")" << std::endl;
	std::cout << "Width in Pixels:   " << getWidth() << std::endl;
	std::cout << "Height in Pixels : " << getHeight() << std::endl;
	std::cout << "Iterations:        " << Function::getBreakoutIterator() << std::endl;
	std::cout << "Gamma value:       " << gamma << std::endl;
	std::cout << "Framerate:         " << getFps() << std::endl;
	std::cout << "Length (Frames):   " << getFrameCount() << std::endl;


	printf("Continue? y/n \n");

	std::cin >> videoToken;
	std::cout << "\033[2J\033[1;1H";

	if (videoToken == 'y')
	{
		std::cin.ignore();
		Julia function;
		std::string userInput;

		StaticColorDomain canvas(getWidth(), getHeight(), getResolution(), getCenterX(), getCenterY());
		
		printf("Enter Expression! \n");
		std::getline(std::cin, userInput);

		if (std::cin.fail())
		{
			printf("Error Reading input! \n");
			return;
		}

		//Prepare the Function
		function.set(userInput);
		function.prepare();
		function.parse();
		
		//Calculate 
		printf("Rendering... \n");
		std::string filename = "ColorDomainAnimation_" + std::to_string(fileNumberMPG) + ".mpg";
		const char* c_filename = filename.c_str();

		auto tstart = steady_clock::now();
		canvas.render (function, 1, 1, c_filename);
		std::cout << "Calculation finished in: " << (double)duration_cast<microseconds>(steady_clock::now() - tstart).count()/1000000.0 << "s" << std::endl;
		
		fileNumberMPG++;
		function.reset();
		canvas.resetBuffer();

		std::cout << "Success! Saved as: " << filename << "!" << std::endl;
		printf("\n");
	}

	
	return;
}

