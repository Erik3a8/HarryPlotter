#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <chrono>
#include <string>
#include <fstream>
#include <filesystem>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <math.h>
#include <memory>
#include <dlfcn.h> 
#include <omp.h>
#include "BaseCanvas.h"
#include "StaticGrid.h"
#include "StaticColorDomain.h"
#include "GeometryCanvas.h"
#include "rgbCanvas.h"

// #include "Expression_SLOW.h"

/* ToDo:
- Implement Bezier Curves
- Color Maps
- Mandelbrot Sets
*/


std::string checkResultsFolderCMD = "[ ! -d \"/Results/\" ] && echo \"not found\" ; mkdir '~/Results'";

std::string clearScreen = "\033[2J\033[1;1H";

void colordomain ()
{
	char colorToken;
	while (colorToken != 'b')
	{
		printf("DOMAIN COLORING MODE \n\n");
		printf("1: Configure \n2: Plot Image \n3: Animate\nb: Back \n");
		std::cin >> colorToken;
		std::cout << clearScreen;
		std::cin.clear();

		switch (colorToken)
		{
			case '1': StaticColorDomain::configure(); break;
			case '2': StaticColorDomain::image (); break;
			case '3': StaticColorDomain::movie (); break;
			case 'b': break;
			default: printf("Invalid Option! \n"); break;
		}
	}
}

void grid ()
{
    char gridToken;
	while (gridToken != 'b')
	{
		printf("GRID MODE \n\n");
		printf("1: Configure \n2: Plot Image \n3: Animate\nb: Back \n");
		std::cin >> gridToken;
		std::cout << clearScreen;
		std::cin.clear();

		switch (gridToken)
		{
			case '1': StaticGrid::configure(); break;
			case '2': StaticGrid::image (); break;
			case '3': StaticGrid::movie (); break;
			case 'b': break;
			default: printf("Invalid Option! \n"); break;
		}
	}
}

void plot ()
{
	char plotToken;
	while (plotToken != 'b')
	{
		printf("COMPLEX PLOTTER \n\n");
		printf("1: Colordomain \n2: Grid \nb: Back \n");
		std::cin >> plotToken;
		std::cout << clearScreen;
		std::cin.clear();

		switch (plotToken)
		{
			case '1': colordomain(); break;
			case '2': grid(); break;
			case 'b': break;
			default: printf("Invalid Option! \n"); break;
		}
	}		
}

void escapeTime ()
{
	char escapeTimeToken;
	while (escapeTimeToken != 'b')
	{
		printf("ESCAPE TIME MODE \n\n");
		printf("1: Configure \n2: Plot Image\nb: Back \n");
		std::cin >> escapeTimeToken;
		std::cout << clearScreen;
		std::cin.clear();

		switch (escapeTimeToken)
		{
			case '1': rgbCanvas::configure(); break;
			case '2': rgbCanvas::image (); break;
			case 'b': break;
			default: printf("Invalid Option! \n"); break;
		}
	}	
}

void geometry ()
{
	char geometryToken;
	while (geometryToken != 'b')
	{
		printf("GEOMETRY MODE \n\n");
		printf("1: Configure \n2: Plot Image\nb: Back \n");
		std::cin >> geometryToken;
		std::cout << clearScreen;
		std::cin.clear();

		switch (geometryToken)
		{
			case '1': GeometryCanvas::configure(); break;
			case '2': GeometryCanvas::image (); break;
			case 'b': break;
			default: printf("Invalid Option! \n"); break;
		}
	}	
}

using namespace std::chrono;

int main() 
{	
	std::cout << clearScreen;

	// Get the home variable (Harry Plotter was installed here)
	std::filesystem::path homeDir = getenv("HOME");
	std::filesystem::path mainDir;

	// Check whether the main folder exists
	if (std::filesystem::exists(homeDir / "HarryPlotter"))
	{
		mainDir = homeDir / "HarryPlotter";
		std::filesystem::current_path(mainDir);
	}
	else
	{
		printf("Main Directory not found, exiting");
		exit(EXIT_FAILURE);
	}


	if (std::filesystem::exists(mainDir / "Results"))
	{
		printf("Found Results directory! \n");
	}
	else
	{
		std::cout << "No Results directory found, creating one at: " << mainDir << "\n" << std::endl;
		std::filesystem::create_directory(mainDir / "Results");
	}

	// Set the directory as the directory for the basecanvas class
	BaseCanvas::setPath((mainDir / "Results").string());

	std::cout.flush();
	std::cout << "Your Output files will be stored in: " << BaseCanvas::getPath() << "\n" << std::endl;


	printf("Welcome to Harry Plotter, the fast and lightweight plotter for complex valued functions and fractals! \n\n");
	char mainToken;
	while (mainToken != 'q')
	{
		printf("MAIN MENU \n\n");
		printf("1: Escape time (Mandelbrot or similar) \n2: Plotting Complex Function \n3: Apollonian Gasket \nq: Exit \n");
		std::cin >> mainToken;
		std::cout << clearScreen;
		std::cin.clear();

		switch (mainToken) 
		{
			case '1': escapeTime (); break;
			case '2': plot (); break;
			case '3': geometry (); break;
			case 'q': break;
			default: printf("Invalid Option! \n"); break;
		}
	}

	printf("Remember to relocate/rename all files of this session or they might be overwritten next time! \n");

	return 0;
}
