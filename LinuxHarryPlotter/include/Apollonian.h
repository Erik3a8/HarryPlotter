/* Class for calculating the centers and radii of the circles comprising an apollonian gasket */

#ifndef APOLLONIUS_H
#define APOLLONIUS_H

#include "Circle.h"
#include <array>


class Apollonius : public Construction {

private:

	/* maximum recursion depth of the tree containing the circles */
	static unsigned int maxRecursionDepth;

	/* setting a minimum curvature to break the recursion ahead of time */
	static double maximumCurvature;

	/* keeping track of how many circles have been drawn so far */
	unsigned int numberOfCircles;

	// Storing 4 circles for each recursive step
	std::array<Circle, 4> recursionArray;

public:

	/* Constructor with the initial curvatures */
	Apollonius (double k1, double k2, double k3);

	~Apollonius();

	static unsigned int getMaxRecursionDepth ();
	static double getMaximumCurvature ();
	unsigned int getNumberOfCircles ();

	static void setMaxRecursionDepth (unsigned int maxRecDepth);
	static void setMaximumCurvature (double maxCurv);
	void setNumberOfCircles (unsigned int numOfCircles);

	double solveQ2 (double c1, double c2, double c3, bool other);
	Complex solveQ2Complex (double k1, double k2, double k3, Complex c1, Complex c2, Complex c3, bool other);

	/* calculate the gasket*/ 
	void recursiveStep (std::array<Circle, 4> circles, unsigned int currentRecursionDepth);
	void evaluate ();
};

#endif
