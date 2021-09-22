/* Class for generating cubic splines for smooth interpolation of int values in any 3D colorspace that I have implemented so far.
Uses the Newton algorithm and then horner evaluation for efficient computation */


#ifndef Interpolator_H
#define Interpolator_H



class Interpolator {
private:

	// coefficients of the splines. 5 cubic splines for each color entry
	double a[5][3], b[5][3], c[5][3], d[5][3];

	// The tridiagonal matrix that will be used
	double helperMatrixR[30], helperMatrixG[30], helperMatrixB[30];

	// x values of the input (colormap)
	double xVal[6];

	// y value of the input (colormap; 0 = R, 1 = G, 2 = B)
	double yVal[6][3];

	// interpolated colorvalues
	double R, G, B; 

public:

	static constexpr unsigned int MAX_CONTROL_POINTS = 6;

	Interpolator () = default;

	// Since this is now Baseclass for rgbCanvas
	virtual ~Interpolator () = default;

	void setxVal (unsigned int n, double val[]);

	void setyValR (unsigned int n, double val[]);
	void setyValG (unsigned int n, double val[]);
	void setyValB (unsigned int n, double val[]);

	void setMAX_CONTROL_POINTS(unsigned int MCP);

	double getR ();
	double getG ();
	double getB ();

	unsigned int getMAX_CONTROL_POINTS ();

	// Order the data into a tridiagonal matrix
	void prepareTridiagMatrix (unsigned int n);

	// usual gaussian elimination
	void gaussianElimination (unsigned int n, unsigned int m, double matrix[], double vector[]);

	// calculate the coefficients
	void findSplineCoeffs (unsigned int n);

	// evaluate the polynomial for a certain parameter
	void hornerEvaluation (unsigned int n, double parameter);


};


#endif