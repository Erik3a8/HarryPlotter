/* Initialize and set data all constant. Benchmark exchanging data instead of reallocating every time for varying
size in case of animations. Definitely observe the deconstructor to avoid memory leaks in said case. */

#include <iostream>
#include <math.h>
#include <chrono>


unsigned int GeometryCanvas::fileNumberBMP{0};
unsigned int GeometryCanvas::fileNumberMPG{0}; 
unsigned int GeometryCanvas::numberOfColors{5};

GeometryCanvas::GeometryCanvas (const int x, const int y, const double res, const double CX, const double CY) 								
{
	this->setResolution(res);
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

GeometryCanvas::~GeometryCanvas ()
{
    // Release allocated memory
	delete []buffer;
}

unsigned int GeometryCanvas::getNumberOfColors ()
{
	return numberOfColors;
}

void GeometryCanvas::setNumberOfColors (unsigned int colors)
{
	numberOfColors = colors;
}


void GeometryCanvas::populateGeoCanvas (Construction constr)
{
    Color rgb(255, 255, 255, "RGB");
    while(not constr.objectQueue.empty())
    {	
    	// Draw the first circle as not filled
    	if (constr.objectQueue.top().getCurvature() < 0)
    	{
    		bool tempFlag = Circle::getFilledFlag(); 
    		Circle::setFilledFlag(false);
    		((constr.objectQueue).top()).draw(rgb, this->buffer, this->getWidth(), this->getHeight());
    		(constr.objectQueue).pop();
    		Circle::setFilledFlag(tempFlag);
    	}

    	// Calculate the correct color 
    	int k, colorType, colorValue;
    	k = (int)constr.objectQueue.top().getCurvature() % numberOfColors;
   		colorType = k % 5;
   		colorValue = ((k - colorType) / 5) % 25;

   		switch(colorType)
   		{
   			case 0: rgb = RED[colorValue]; break;
   			case 1: rgb = GREEN[colorValue]; break;
   			case 2: rgb = BLUE[colorValue]; break;
   			case 3: rgb = YELLOW[colorValue]; break;
   			case 4: rgb = MAGENTA[colorValue]; break;
   		}

   		// draw the circle
        ((constr.objectQueue).top()).draw(rgb, this->buffer, this->getWidth(), this->getHeight());
        (constr.objectQueue).pop();
    }
}

// So begins the enormous command line interface...
void GeometryCanvas::configure ()
{
	char configToken;
	while (configToken != 'b')
	{
		std::cin.ignore();
		printf("CONFIGURE \n\n");
		printf("1: Set width and height in pixels \n");
		printf("2: Set width and height in real size \n");
		printf("3: Set center \n");
		printf("4: Set maximum Recursion depth \n");
		printf("5: Set Filled circles \n");
		printf("6: Set framerate \n");
		printf("7: Set length in seconds \n");
		printf("8: Set path \n");
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
				printf("Enter desired recursion depth! \n");
				unsigned int depth;

				std::cin >> depth;
				std::cin.clear();
				std::cout << "\033[2J\033[1;1H";

				if (depth <= 0)
				{
					printf("Bad input, must be greater or equal to 0! \n\n");
					break;
				}

				Apollonius::setMaxRecursionDepth(depth);
				printf("Maximum recursion depth set to %u \n\n", Apollonius::getMaxRecursionDepth());
				break;

			case '5': 
				printf("Toggle filled circles y/n? \n");
				char toggle;
				std::cin >> toggle;
				std::cin.clear();
				std::cout << "\033[2J\033[1;1H";

				switch(toggle)
				{
					case 'y': 
						unsigned int colors;
						Circle::setFilledFlag(true);
						printf("Drawing filled circles!, Enter desired number of colors! \n");
						std::cin >> colors;
						std::cin.clear();
						std::cout << "\033[2J\033[1;1H";
						setNumberOfColors(colors);
						break;

					case 'n':
						Circle::setFilledFlag(false);
						printf("Drawing outlines only! \n");
						break;

					default: 
						printf("Invalid Option! \n");
						break;
				}
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

			// case '8':
			// 	printf("Enter desired length in seconds! \n\n");
			// 	double userLength;

			// 	std::cin >> userLength;
			// 	std::cin.clear();
			// 	std::cout << "\033[2J\033[1;1H";

			// 	if (userLength <= 0)
			// 	{
			// 		printf("Bad input, must be a positive number! \n\n");
			// 		return;
			// 	}

			// 	SET_FRAME_COUNT(userLength * GET_FPS());
			// 	printf("Set length to %d frames \n\n", GET_FRAME_COUNT()); 
			// 	break;

			case 'b': break;

			default: printf("Invalid Option! \n\n"); break;
		}
	}
}

void GeometryCanvas::image ()
{
	char imageToken;
	printf("PLOTTER \n\n");
	printf("This function will output a .bmp file with the following properties: \n");

	// Inform the user about the output that will be created
	std::cout << "Center point :     " << "(" << getCenterX() << ", " << getCenterY() << ")" << std::endl;
	std::cout << "Width in Pixels:   " << getWidth() << std::endl;
	std::cout << "Height in Pixels : " << getHeight() << std::endl;
	std::cout << "Maximum Recursion: " << Apollonius::getMaxRecursionDepth() << std::endl;
	std::cout << "Filled Circles:    " << Circle::getFilledFlag() << std::endl;

	printf("Continue? y/n \n");

	std::cin >> imageToken;
	std::cout << "\033[2J\033[1;1H"; 

	if(imageToken == 'y')
	{
		std::cin.ignore();
		double k1, k2, k3;
		printf("Enter initial curvatures! \n");
		std::cin >> k1;
		std::cin >> k2;
		std::cin >> k3;

		GeometryCanvas canvas(getWidth(), getHeight(), getResolution(), getCenterX(), getCenterY());
		//Calculate 
		auto tstart = steady_clock::now();

		printf("Calculating... \n");
		Apollonius Gasket(k1, k2, k3);
		Gasket.evaluate();

		canvas.populateGeoCanvas(Gasket);
		std::cout << "Calculation finished in: " << (double)duration_cast<microseconds>(steady_clock::now() - tstart).count()/1000000.0 << "s" << std::endl;

		printf("Writing file... \n");

		std::string filename = "ApollonianGasket_" + std::to_string(fileNumberBMP) + ".bmp";
		const char* c_filename = filename.c_str();
		canvas.saveBMP(c_filename);

		fileNumberBMP++;

		canvas.resetBuffer();

		std::cout << "Success! Saved as: " << filename << "!" << std::endl;
		printf("\n");
	}
	return;
}

// void GeometryCanvas::movie ()
//{
// 	char videoToken;
//	printf("ANIMATOR \n\n");
//	printf("This function will output a .mpg file with the following properties: \n");
//
//	// Inform the user about the output that will be created
//	std::cout << "Center point :     " << "(" << getCenterX() << ", " << getCenterY() << ")" << std::endl;
//	std::cout << "Width in Pixels:   " << getWidth() << std::endl;
//	std::cout << "Height in Pixels : " << getHeight() << std::endl;
//	std::cout << "Iterations:        " << Function::getBreakoutIterator() << std::endl;
//	std::cout << "Gamma value:       " << gamma << std::endl;
//	std::cout << "Framerate:         " << getFps() << std::endl;
//	std::cout << "Length (Frames):   " << getFrameCount() << std::endl;
//
//
//	printf("Continue? y/n \n");
//
//	std::cin >> videoToken;
//	std::cout << "\033[2J\033[1;1H";
//
//	if (videoToken == 'y')
//	{
//		std::cin.ignore();
//		Julia function;
//		std::string userInput;
//
//		GeometryCanvas canvas(getWidth(), getHeight(), getResolution(), getCenterX(), getCenterY());
//		
//		printf("Enter Expression! \n");
//		std::getline(std::cin, userInput);
//
//		if (std::cin.fail())
//		{
//			printf("Error Reading input! \n");
//			return;
//		}
//
//		//Prepare the Function
//		function.set(userInput);
//		function.prepare();
//		function.parse();
//
//		if (function.getFuncPtr() == nullptr)
//		{
//			printf("Exiting... \n\n");
//			return;
//		}
//
//		//Calculate 
//		printf("Rendering... \n");
//		std::string filename = "ColorDomainAnimation_" + std::to_string(fileNumberMPG) + ".mpg";
//		const char* c_filename = filename.c_str();
//
//		auto tstart = steady_clock::now();
//		canvas.render (function, 1, 1, c_filename);
//		std::cout << "Calculation finished in: " << (double)duration_cast<microseconds>(steady_clock::now() - tstart).count()/1000000.0 << "s" << std::endl;
//		
//		fileNumberMPG++;
//		function.reset();
//		canvas.resetBuffer();
//
//		std::cout << "Success! Saved as: " << filename << "!" << std::endl;
//		printf("\n");
//	}
//
//	
//	return;
//}
