#include <cstdio>
#include <cstdlib>
#include <iostream>
#include "Interpolator.h"

// For testing and debugging
void printMatrix(int m, int n, double matrix[]){
    int i, j;
    for(i = 0; i < m; i++)
    {
        for(j = 0; j < n; j++)
        {
            printf("%lf\t",matrix[j * m + i]);
        }
        printf("\n");
    } 
}

void Interpolator::setyValR (unsigned int n, double val[])
{
	if (n > MAX_CONTROL_POINTS)
	{
		printf("Error, too many colorvalues! \n");
		return;
	}

	for (int i = 0; i < n; i++)
	{
		this->yVal[i][0] = val[i];	
	}
} 

void Interpolator::setyValG (unsigned int n, double val[])
{
	if (n > MAX_CONTROL_POINTS)
	{
		printf("Error, too many colorvalues! \n");
		return;
	}

	for (int i = 0; i < n; i++)
	{
		this->yVal[i][1] = val[i];	
	}
} 

void Interpolator::setyValB (unsigned int n, double val[])
{
	if (n > MAX_CONTROL_POINTS)
	{
		printf("Error, too many colorvalues! \n");
		return;
	}

	for (int i = 0; i < n; i++)
	{
		this->yVal[i][2] = val[i];	
	}
} 


void Interpolator::setxVal (unsigned int n, double val[])
{
	if (n > MAX_CONTROL_POINTS)
	{
		printf("Error, too many colorpoints! \n");
		return;
	}

	for (int i = 0; i < n; i++)
	{
		this->xVal[i] = val[i];	
	}
}


double Interpolator::getR ()
{
	return this->R; 
}

double Interpolator::getG ()
{
	return this->G; 
}

double Interpolator::getB ()
{
	return this->B; 
}


// this has to be done only once, luckily auto vectorization works fine with g++ (n = MAX_CONTROL_POINTS - 1)
void Interpolator::prepareTridiagMatrix (unsigned int n)
{	
	// Initialize the helper matrices as all zero to avoid numeric oddities
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n - 1; j++)
		{
			helperMatrixR[j + n * i] = 0;
			helperMatrixG[j + n * i] = 0;
			helperMatrixB[j + n * i] = 0;
		}
	}

	// Initialize the interval widths of the x values
	double intervalWidths[n];
	for (int i = 0; i < n; i++)
	{
		intervalWidths[i] = xVal[i + 1] - xVal[i];
	}


	// populate the helper matrices
	for (int i = 0; i < n - 1; i++)
	{
		helperMatrixR[i * (n - 1) + i] = 2 * (intervalWidths[i] + intervalWidths[i + 1]);
		helperMatrixG[i * (n - 1) + i] = 2 * (intervalWidths[i] + intervalWidths[i + 1]);
		helperMatrixB[i * (n - 1) + i] = 2 * (intervalWidths[i] + intervalWidths[i + 1]);
	}

	for (int i = 0; i < n - 2; i++)
	{
		helperMatrixR[(i + 1) * (n - 1) + i] = intervalWidths[i + 1];
		helperMatrixG[(i + 1) * (n - 1) + i] = intervalWidths[i + 1];
		helperMatrixB[(i + 1) * (n - 1) + i] = intervalWidths[i + 1];

		helperMatrixR[i * (n - 1) + i + 1] = intervalWidths[i + 1];
		helperMatrixG[i * (n - 1) + i + 1] = intervalWidths[i + 1];
		helperMatrixB[i * (n - 1) + i + 1] = intervalWidths[i + 1];
	}

	for (int i = 1; i < n; i++)
	{
		helperMatrixR[(n - 1) * (n - 1) + (i - 1)] = (yVal[i + 1][0] - yVal[i][0]) * 6 / (double)intervalWidths[i] - (yVal[i][0] - yVal[i - 1][0]) * 6 / (double)intervalWidths[i - 1];
		helperMatrixG[(n - 1) * (n - 1) + (i - 1)] = (yVal[i + 1][1] - yVal[i][1]) * 6 / (double)intervalWidths[i] - (yVal[i][1] - yVal[i - 1][1]) * 6 / (double)intervalWidths[i - 1];
		helperMatrixB[(n - 1) * (n - 1) + (i - 1)] = (yVal[i + 1][2] - yVal[i][2]) * 6 / (double)intervalWidths[i] - (yVal[i][2] - yVal[i - 1][2]) * 6 / (double)intervalWidths[i - 1];
	}

	// printMatrix(4, 5, helperMatrixG);
}

void Interpolator::gaussianElimination (unsigned int m, unsigned int n, double matrix[], double vector[])
{
    for(int i = 0; i < m - 1 ; i++)
    {
        //Begin Gauss Elimination
        for (int k = i + 1; k < m; k++)
        {
            double factor = matrix[i * m + k] / matrix[i * m + i];
            for (int j = 0; j < n; j++)
            {
                matrix[j * m + k] = matrix[j * m + k] - factor * matrix[j * m + i];
            }
        } 
    }

    // Back-substitution
    for (int i = m - 1; i >= 0; i--)
    {
        vector[i] = matrix[(n - 1) * m + i];
        for (int j = i + 1; j < n - 1; j++)
        {
            vector[i] = vector[i] - matrix[j * m + i] * vector[j];
        }
        vector[i] = vector[i] / matrix[i * m + i];
    }         
}

void Interpolator::findSplineCoeffs (unsigned int n)
{
	// This is the vector that arises from the gauss elimination, the "short" version is the one that actually gets used for elimination
	double tempR[n + 1], tempG[n + 1], tempB[n + 1], shortTempR[n - 1], shortTempG[n - 1], shortTempB[n - 1];
	double intervalWidths[n];

	// Inefficient to calculate this again but so what... this will only happen once
	for (int i = 0; i < n; i++)
	{
		intervalWidths[i] = xVal[i + 1] - xVal[i];
	}

	gaussianElimination (n - 1, n, helperMatrixR, shortTempR);
	gaussianElimination (n - 1, n, helperMatrixG, shortTempG);
	gaussianElimination (n - 1, n, helperMatrixB, shortTempB);

	// I am using natural splines
	tempR[0] = tempG[0] = tempB[0] = tempR[n] = tempG[n] = tempB[n] = 0;

	// Put the data into the actual temp vectors
	for (int i = 1; i < n; i++)
	{
		tempR[i] = shortTempR[i - 1];
		tempG[i] = shortTempG[i - 1];
		tempB[i] = shortTempB[i - 1];
	}

	// Calculate the coefficients
	for (int i = 0; i < n; i++)
	{
		d[i][0] = yVal[i][0];
		d[i][1] = yVal[i][1];
		d[i][2] = yVal[i][2];

		b[i][0] = tempR[i] / 2;
		b[i][1] = tempG[i] / 2;
		b[i][2] = tempB[i] / 2;

		a[i][0] = (tempR[i + 1] - tempR[i]) / (intervalWidths[i] * 6);
		a[i][1] = (tempG[i + 1] - tempG[i]) / (intervalWidths[i] * 6);
		a[i][2] = (tempB[i + 1] - tempB[i]) / (intervalWidths[i] * 6);

		c[i][0] = (yVal[i + 1][0] - yVal[i][0]) / intervalWidths[i] - intervalWidths[i] * (2 * tempR[i] + tempR[i + 1]) / 6;
		c[i][1] = (yVal[i + 1][1] - yVal[i][1]) / intervalWidths[i] - intervalWidths[i] * (2 * tempG[i] + tempG[i + 1]) / 6;
		c[i][2] = (yVal[i + 1][2] - yVal[i][2]) / intervalWidths[i] - intervalWidths[i] * (2 * tempB[i] + tempB[i + 1]) / 6;
	}
}


// this happens a lot of times. Also vectorization works very well (-msse3, -Ofast), also only cubics!
void Interpolator::hornerEvaluation (unsigned int n, double parameter)
{
	// read the coefficients into more convenient vectors (4 coefficients for n - 1 splines)
	double coeffR[4][n - 1], coeffG[4][n - 1], coeffB[4][n - 1];

	for (int i = 0; i < n - 1; i++)
	{
		coeffR[3][i] = a[i][0];
		coeffR[2][i] = b[i][0];
		coeffR[1][i] = c[i][0];
		coeffR[0][i] = d[i][0];

		coeffG[3][i] = a[i][1];
		coeffG[2][i] = b[i][1];
		coeffG[1][i] = c[i][1];
		coeffG[0][i] = d[i][1];

		coeffB[3][i] = a[i][2];
		coeffB[2][i] = b[i][2];
		coeffB[1][i] = c[i][2];
		coeffB[0][i] = d[i][2];

	}

	// Determine which cubic to evaluate for said parameter
	for (int i = 0; i < n; i++)
	{
		if (n > MAX_CONTROL_POINTS)
		{
			printf("Error, too many colorvalues! \n");
			return;
		}

		if ((xVal[i] <= parameter and parameter < xVal[i + 1]) or parameter == xVal[n])
		{	
	 		double sumR, sumG, sumB;
  			double factor = 1;

  			sumR = sumG = sumB = 0;

    		for (int j = 0; j < n; j++) 
   	 		{
    			sumR += factor * coeffR[j][i];
    			sumG += factor * coeffG[j][i];
    			sumB += factor * coeffB[j][i];
    			factor *= (parameter - xVal[i]);
    		}
    		
    		// Making sure we do not go higher or lower than 0 and 255 respectively
    		this->R = std::clamp((int)sumR, 0, UINT8_MAX);
    		this->G = std::clamp((int)sumG, 0, UINT8_MAX);
    		this->B = std::clamp((int)sumB, 0, UINT8_MAX);
		}

		else 
		{
			continue;
		}
	}
}








