#include "Apollonian.h"
#include "Construction.h"
#include "GeoObject.h"
#include "BaseCanvas.h"
#include <stack>

unsigned int Apollonius::maxRecursionDepth{1};
double Apollonius::maximumCurvature{10000};


Apollonius::Apollonius (double k1, double k2, double k3)
{	
	// checking for bad values (less than zero radii or the inner circles are bigger than the outer circle)
	if (k1 <= 0 || k2 <= 0 || k3 <= 0)
	{
		printf("Bad values! \n");
		k1 = k2 = k3 = 2;
	}

	// calculating the radii from the curvatures
	double radius1 = 1 / k1, radius2 = 1 / k2, radius3 = 1 / k3, radius4;
	Complex center1, center2, center3, center4;

	double k4 = solveQ2(k1, k2, k3, true);
	radius4 = abs(1 / k4);
	

	// Calculating the height of the triangle spanned by the circle centerpoints to determine them (based on the radii)
	double s = 0.5 * ((radius1 + radius2) + (radius2 + radius3) + (radius1 + radius3));
	double height = 2 * sqrt(s * (s - (radius1 + radius2)) * (s - (radius1 + radius3)) * (s - (radius2 + radius3))) / (radius1 + radius2);

	// setting the centers
	center1 = Complex(0, 0);
	center2 = Complex(radius1 + radius2, 0);
	center3 = Complex(sqrt((radius1 + radius3) * (radius1 + radius3) - height * height), height);
	center4 = solveQ2Complex(k1, k2, k3, center1, center2, center3, true) / Complex(k4, 0);

	// Centering on the origin
	center1 = center1 - center4;
	center2 = center2 - center4;
	center3 = center3 - center4;
	center4 = Complex(0, 0); 

	// setting the circles
	Circle circle1(radius1, center1);
	Circle circle2(radius2, center2);
	Circle circle3(radius3, center3);
	Circle circle4(radius4, center4);

	circle4.setCurvature(k4);

	this->objectQueue.push(circle1);
	this->objectQueue.push(circle2);
	this->objectQueue.push(circle3);
	this->objectQueue.push(circle4);

	// Setting the recursion array
	this->recursionArray = {circle1, circle2, circle3, circle4};
}

Apollonius::~Apollonius () {}

double Apollonius::getMaximumCurvature ()
{
	return maximumCurvature;
}

unsigned int Apollonius::getMaxRecursionDepth ()
{
	return maxRecursionDepth;
}

unsigned int Apollonius::getNumberOfCircles ()
{
	return this->numberOfCircles;
}


void Apollonius::setMaxRecursionDepth (unsigned int maxRecDepth)
{
	maxRecursionDepth = maxRecDepth;
}

void Apollonius::setMaximumCurvature (double maxCurv)
{
	maximumCurvature = maxCurv;
}

void Apollonius::setNumberOfCircles (unsigned int numOfCircles)
{
	this->numberOfCircles = numOfCircles;
}

double Apollonius::solveQ2 (double k1, double k2, double k3, bool other)
{
	if (other)
	{
		double k4 = k1 + k2 + k3 - 2 * sqrt(k1 * k2 + k2 * k3 + k3 * k1);
		return k4;
	}

	double k4 = k1 + k2 + k3 + 2 * sqrt(k1 * k2 + k2 * k3 + k3 * k1);
	return k4;
}

Complex Apollonius::solveQ2Complex (double k1, double k2, double k3, Complex c1, Complex c2, Complex c3, bool other)
{
	Complex c1k1, c2k2, c3k3;
	c1k1 = Complex(k1, 0) * c1;
	c2k2 = Complex(k2, 0) * c2;
	c3k3 = Complex(k3, 0) * c3;

	if (other)
	{
		Complex c4k4 = c1k1 + c2k2 + c3k3 - Complex(2, 0) * (c1k1 * c2k2 + c3k3 * c2k2 + c1k1 * c3k3).csqrt();
		return c4k4;
	}

	Complex c4k4 = c1k1 + c2k2 + c3k3 + Complex(2, 0) * (c1k1 * c2k2 + c3k3 * c2k2 + c1k1 * c3k3).csqrt();
	return c4k4;
}

void Apollonius::recursiveStep (std::array<Circle, 4> circles, unsigned int currentRecursionDepth)
{
	if (currentRecursionDepth == maxRecursionDepth)
	{
		return;
	}

	Circle newCirc; 
	double curv[4];
	Complex centers[4];

	// for readability (if this kills performance, remove it instantly)
	for (int i = 0; i < 4; i++)
	{
		centers[i] = circles[i].getCenter();
		curv[i] = circles[i].getCurvature();
	}

	// using Descartes and Lagarias formula to find the new radius and the new centerpoint 
	double newCurv = 2 * (curv[0] + curv[1] + curv[2]) - curv[3];
	double newRadius = abs(1 / newCurv);
	Complex newCenter = (Complex(2, 0) * (Complex(curv[0], 0) * centers[0] + Complex(curv[1], 0) * centers[1] + Complex(curv[2], 0) * centers[2]) - Complex(curv[3], 0) * centers[3]) / Complex(newCurv, 0);
	
	// setting the new circle and adding it to the recursion vector
	newCirc = Circle(newRadius, newCenter);
	newCirc.setCurvature(newCurv);

	recursiveStep({circles[0], circles[1], newCirc, circles[2]}, currentRecursionDepth + 1);
	recursiveStep({circles[0], circles[2], newCirc, circles[1]}, currentRecursionDepth + 1);
	recursiveStep({circles[1], circles[2], newCirc, circles[0]}, currentRecursionDepth + 1);

	this->objectQueue.push(newCirc);
}

void Apollonius::evaluate ()
{
	recursiveStep({recursionArray[0], recursionArray[1], recursionArray[2], recursionArray[3]}, 0);
	recursiveStep({recursionArray[3], recursionArray[1], recursionArray[2], recursionArray[0]}, 0);
	recursiveStep({recursionArray[0], recursionArray[2], recursionArray[3], recursionArray[1]}, 0);
	recursiveStep({recursionArray[0], recursionArray[1], recursionArray[3], recursionArray[2]}, 0);
	return;
}

