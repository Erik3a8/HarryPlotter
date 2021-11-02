/* Initialize and set data all constant. Benchmark exchanging data instead of reallocating every time for varying
size in case of animations. Definitely observe the deconstructor to avoid memory leaks in said case. */
#include <iostream>
#include <math.h>
#include <chrono>
#include "StaticGrid.h"
#include "Julia.h"



unsigned int StaticGrid::lineSpacing{10};
unsigned int StaticGrid::fileNumberBMP{0};
unsigned int StaticGrid::fileNumberMPG{0};

StaticGrid::StaticGrid (const int x, const int y, const double res, const double CX, const double CY) 								
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

StaticGrid::~StaticGrid ()
{
    // Release allocated memory
	delete []buffer;
}

double StaticGrid::findSamplingStepX (const int X, const int Y, Function &F)
{
    double re = (double)(getCenterX() - realSizeX / 2) + (X - 0.5) * getResolution();
    double im = (double)(getCenterY() + realSizeY / 2) - (Y) * getResolution();

    F.setInput(Complex(re, im));
    Complex start = F.returnComplex();

    re = (double)(getCenterX() - realSizeX / 2) + (X + 0.5) * getResolution();
    im = (double)(getCenterY() + realSizeY / 2) - (Y) * getResolution();

    F.setInput(Complex(re, im));
    Complex end = F.returnComplex();

    return getResolution() / (start - end).norm();
}

double StaticGrid::findSamplingStepY (const int X, const int Y, Function &F)
{   
    double re = (double)(getCenterX() - realSizeX / 2) + (X) * getResolution();
    double im = (double)(getCenterY() + realSizeY / 2) - (Y - 0.5) * getResolution();

    F.setInput(Complex(re, im));
    Complex start = F.returnComplex();

    re = (double)(getCenterX() - realSizeX / 2) + (X) * getResolution();
    im = (double)(getCenterY() + realSizeY / 2) - (Y + 0.5) * getResolution();

    F.setInput(Complex(re, im));
    Complex end = F.returnComplex();

    return getResolution() / (start - end).norm();
}

void StaticGrid::populatePixel (const int X, const int Y, Function &F)                         
{   
    if (samplingDirection)
    {
        double stepX = findSamplingStepX(X, Y, F);

        if (isnan(stepX))
        {
            stepX = 0.00001;
        }

        for (double superSampleX = -0.5; superSampleX < 0.5; superSampleX += stepX)
        {
            // Convert Pixels to actual Complex values
            double re = (double)(getCenterX() - realSizeX / 2) + (X + superSampleX) * getResolution();
            double im = (double)(getCenterY() + realSizeY / 2) - (Y) * getResolution();

            // Set the function to that Pixel.
            F.setValue(Complex(re, im));
            F.setInput(Complex(re, im));
            
            // Compute the value of the function
            Complex zMapped = F.returnComplex();

            // Compute the Corresponding Pixel
            int X_NEW = round((zMapped.Re() - (getCenterX() - realSizeX / 2)) / getResolution());
            int Y_NEW = round(((getCenterY() + realSizeY / 2) - zMapped.Im()) / getResolution());

            // Check if that Pixel is outside the Canvas
            if (X_NEW >= getWidth() || Y_NEW >= getHeight() || Y_NEW < 0 || X_NEW < 0 || Y_NEW * getWidth() + X_NEW > getHeight() * getWidth() + getWidth() - 1)
            {
                return;  
            }

            // Convert the Pixel to a Color Object
            Color Pixel(360, 1, 1, "HSL");
            Pixel.HSL_to_RGB();

            // Write to the Buffer
            buffer[Y_NEW * getWidth() + X_NEW] = Pixel;
        }
    }

    else 
    {
        double stepY = findSamplingStepY(X, Y, F);

        if (isnan(stepY))
        {
            stepY = 0.00001;
        }

        for (double superSampleY = -0.5; superSampleY < 0.5; superSampleY += stepY)
        {
            // Convert Pixels to actual Complex values
            double re = (double)(getCenterX() - realSizeX / 2) + X * getResolution();
            double im = (double)(getCenterY() + realSizeY / 2) - (Y - superSampleY) * getResolution();

            // Set the function to that Pixel.
            F.setValue(Complex(re, im));
            F.setInput(Complex(re, im));
            
            // Compute the value of the function
            Complex zMapped = F.returnComplex();

            // Compute the Corresponding Pixel
            int X_NEW = round((zMapped.Re() - (getCenterX() - realSizeX / 2)) / getResolution());
            int Y_NEW = round(((getCenterY() + realSizeY / 2) - zMapped.Im()) / getResolution());

            // Check if that Pixel is outside the Canvas
            if (X_NEW >= getWidth() || Y_NEW >= getHeight() || Y_NEW < 0 || X_NEW < 0 || Y_NEW * getWidth() + X_NEW > getHeight() * getWidth() + getWidth() - 1)
            {
                return;  
            }

            // Convert the Pixel to a Color Object
            Color Pixel(360, 1, 1, "HSL");
            Pixel.HSL_to_RGB();

            // Write to the Buffer
            buffer[Y_NEW * getWidth() + X_NEW] = Pixel;
        }
    }
      
}

void StaticGrid::populateCanvas (Function &F, const int interval) 
{
    unsigned int reducedHeight, reducedWidth;
    reducedHeight = getHeight() / interval;
    reducedWidth = getWidth() / interval;

    /* Filling the buffer with color data. Using multicore power TL-BR */
    this->samplingDirection = true;

#pragma omp parallel for collapse(2)
    for (int Y = reducedHeight - 1; Y >= 0; Y--) 
    {
        for (int X = 0; X < getWidth(); X++) 
        {
            this->populatePixel(X, Y * interval, *F.cloneFunction());
        }
    }
    
    this->samplingDirection = false;

#pragma omp parallel for collapse(2)
    for (int Y = getHeight() - 1; Y >= 0; Y--) 
    {
        for (int X = 0; X < reducedWidth; X++) 
        { 
            this->populatePixel(X * interval, Y, *F.cloneFunction()); 
        }
    }
}

using namespace std::chrono;

void StaticGrid::configure ()
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
        printf("5: Set line spacing \n");
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
                printf("Enter desired line spacing! \n");
                int userSpacing;

                std::cin >> userSpacing;
                std::cin.clear(); 
                std::cout << "\033[2J\033[1;1H";

                // Rule out negative values or line spacing that is bigger than the frame itself
                if (userSpacing <= 0 || std::cin.fail() || userSpacing > getHeight() || userSpacing > getWidth())
                {
                    printf("Bad input, must be integer greater than 0! \n");
                    return;
                }
                lineSpacing = userSpacing;
                printf("Set line spacing to %d \n\n", lineSpacing);
                break;

            case '6': 
                printf("Enter desired framerate (Only 24, 25, 30, 50, or 60)! \n");
                int userFps;

                std::cin >> userFps;
                std::cin.clear();       
                std::cout << "\033[2J\033[1;1H";    

                // Only those values are supported by the encoder
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
                printf("Enter desired length in seconds! \n\n");
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

void StaticGrid::image ()
{
    char IMAGE_TOKEN;
    printf("PLOTTER \n\n");
    printf("This function will output a .bmp file with the following properties: \n");

    // Inform the user about the output that will be created
    std::cout << "Center point :     " << "(" << getCenterX() << ", " << getCenterY() << ")" << std::endl;
    std::cout << "Width in Pixels:   " << getWidth() << std::endl;
    std::cout << "Height in Pixels : " << getHeight() << std::endl;
    std::cout << "iterations:        " << Function::getBreakoutIterator() << std::endl;
    std::cout << "Line Spacing:      " << lineSpacing << std::endl;

    printf("Continue? y/n \n");

    std::cin >> IMAGE_TOKEN;
    std::cout << "\033[2J\033[1;1H";

    if(IMAGE_TOKEN == 'y')
    {
        std::cin.ignore();
        Julia function;
        std::string userInput;

        StaticGrid canvas(getWidth(), getHeight(), getResolution(), getCenterX(), getCenterY());

        printf("Enter Expression! \n");
        std::getline(std::cin, userInput);

        if (std::cin.fail())
        {
            printf("Error Reading input! \n");
            return;
        }

        //Prepare the function
        function.set(userInput);
        function.prepare();
        function.parse();

        if (function.getFuncPtr() == nullptr)
        {
            printf("Exiting... \n\n");
            return;
        }

        //Calculate 
        printf("Calculating... \n");

        auto tstart = steady_clock::now();
        canvas.populateCanvas(function, lineSpacing);
        std::cout << "Calculation finished in: " << (double)duration_cast<microseconds>(steady_clock::now() - tstart).count()/1000000.0 << "s" << std::endl;
        
        printf("Writing file... \n");
        std::string filename = "Grid_" + std::to_string(fileNumberBMP) + ".bmp";
        const char* c_filename = filename.c_str();
        canvas.saveBMP(c_filename);

        // Paint it black
        canvas.resetBuffer();
        function.reset();
        // Done

        fileNumberBMP++;
        std::cout << "Success! Saved as: " << filename << "!" << std::endl;
        printf("\n");
    }
    return;
}

void StaticGrid::movie ()
{
    char videoToken;
    printf("ANIMATOR \n\n");
    printf("This function will output a .mpg file with the following properties: \n");

    // Inform the user about the output that will be created
    std::cout << "Center point :     " << "(" << getCenterX() << ", " << getCenterY() << ")" << std::endl;
    std::cout << "Width in Pixels:   " << getWidth() << std::endl;
    std::cout << "Height in Pixels : " << getHeight() << std::endl;
    std::cout << "Line spacing:      " << lineSpacing << std::endl;
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

        StaticGrid canvas(getWidth(), getHeight(), getResolution(), getCenterX(), getCenterY());
        
        printf("Enter Expression! \n");
        std::getline(std::cin, userInput);

        if (std::cin.fail())
        {
            printf("Error Reading input! \n");
            return;
        }

        //Prepare the function
        function.set(userInput);
        function.prepare();
        function.parse();

        if (function.getFuncPtr() == nullptr)
        {
            printf("Exiting... \n\n");
            return;
        }

        //Calculate 
        printf("Rendering... \n");
        std::string filename = "GridAnimation_" + std::to_string(fileNumberMPG) + ".mpg";
        const char* c_filename = filename.c_str();

        auto tstart = steady_clock::now();
        canvas.render(function, lineSpacing, 0, c_filename);
        std::cout << "Calculation finished in: " << (double)duration_cast<microseconds>(steady_clock::now() - tstart).count()/1000000.0 << "s" << std::endl;
       
        function.reset();
        canvas.resetBuffer();

        fileNumberMPG++;
        std::cout << "Success! Saved as: " << filename << "!" << std::endl;
        printf("\n");
    }

    
    return;
}





