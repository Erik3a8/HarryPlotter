
#include "BaseCanvas.h"
#include "GeometryCanvas.h"

bool Circle::filledFlag{true};


Circle::Circle (double radius, Complex center)
{
	this->parameter1 = radius;
	this->parameter2 = 1 / radius;
	this->parameter3 = center.Re();
	this->parameter4 = center.Im();
	if (radius < 0)
	{
		printf("Bad radius! \n");
		this->parameter1 = 0;
	}
}

bool Circle::operator== (const Circle circ)
{
	if (this->parameter1 == circ.parameter1 && this->parameter3 == circ.parameter3 && this->parameter4 == circ.parameter4)
	{
		return true;
	}
	return false;
}

// void Circle::bresenham (int xc, int yc, int x, int y, Color rgb, Color buffer[], unsigned int bufferWidth, unsigned int bufferHeight)
// {
// 	if (xc >= bufferWidth - 1 || yc >= bufferHeight - 1 || xc + x >= bufferWidth - 1 || yc + y >= bufferHeight - 1 || yc - y < 0 || xc - x < 0)
// 	{
// 		return;
// 	}

// 	buffer[(yc + y) * bufferWidth + (xc + x)] = rgb;
// 	buffer[(yc - y) * bufferWidth + (xc + x)] = rgb;
// 	buffer[(yc + y) * bufferWidth + (xc - x)] = rgb;
// 	buffer[(yc - y) * bufferWidth + (xc - x)] = rgb;
// 	buffer[(yc + x) * bufferWidth + (xc + y)] = rgb;
// 	buffer[(yc - x) * bufferWidth + (xc + y)] = rgb;
// 	buffer[(yc + x) * bufferWidth + (xc - y)] = rgb;
// 	buffer[(yc - x) * bufferWidth + (xc - y)] = rgb;
// }

void Circle::setCenter (Complex center)
{
	this->parameter3 = center.Re();
	this->parameter4 = center.Im();
}

void Circle::setRadius (double radius)
{
	this->parameter1 = radius;
}

void Circle::setCurvature (double curvature)
{
	this->parameter2 = curvature;
}

void Circle::setFilledFlag (bool filled)
{
	filledFlag = filled;
}


Complex Circle::getCenter ()
{
	return Complex(this->parameter3, this->parameter4);
} 

double Circle::getRadius ()
{
	return this->parameter1;
}

double Circle::getCurvature ()
{
	return this->parameter2;
}

bool Circle::getFilledFlag ()
{
	return filledFlag;
}

// void Circle::draw (Color rgb, Color buffer[], unsigned int bufferWidth, unsigned int bufferHeight)
// {
// 	// std::cout << "Called circle drawing! \n" << std::endl;
// 	unsigned int canvasX, canvasY, canvasRadius;

// 	canvasX = round((this->parameter3 - ((BaseCanvas::getCenterX() - BaseCanvas::getWidth()) * BaseCanvas::getResolution() / 2)) / BaseCanvas::getResolution());
// 	canvasY = round((((BaseCanvas::getCenterY() + BaseCanvas::getHeight()) * BaseCanvas::getResolution() / 2) - this->parameter4) / BaseCanvas::getResolution());

// 	Complex(parameter3, parameter4).print();
// 	printf("\n");
// 	std::cout << "curvature: " << this->parameter2 << std::endl;

// 	canvasRadius = round(this->parameter1 / BaseCanvas::getResolution());

// 	int x = 0, y = canvasRadius, decision = 3 - 2 * canvasRadius;
// 	bresenham(canvasX, canvasY, x, y, rgb, buffer, BaseCanvas::getWidth(), BaseCanvas::getHeight());

// 	while (y >= x)
// 	{
// 		x++;

// 		if (decision > 0)
// 		{
// 			y--;
// 			decision = decision + 4 * (x - y) + 10;
// 		}
// 		else 
// 		{
// 			decision = decision + 4 * x + 6;
// 		}
// 		bresenham(canvasX, canvasY, x, y, rgb, buffer, BaseCanvas::getWidth(), BaseCanvas::getHeight());
// 	}
// }

void Circle::draw (Color rgb, Color buffer[], unsigned int bufferWidth, unsigned int bufferHeight)
{
	int canvasX, canvasY, canvasRadius, x, y;

	canvasX = round((this->parameter3 - (BaseCanvas::getCenterX() - BaseCanvas::getWidth() * BaseCanvas::getResolution()  / 2)) / BaseCanvas::getResolution());
	canvasY = round((((BaseCanvas::getCenterY() + BaseCanvas::getHeight() * BaseCanvas::getResolution()  / 2)) - this->parameter4) / BaseCanvas::getResolution());

	// Complex(parameter3, parameter4).print();
	// std::cout << canvasX << ", " << canvasY << std::endl;
	// printf("\n");
	// std::cout << "curvature: " << this->parameter2 << std::endl;

	canvasRadius = round(this->parameter1 / BaseCanvas::getResolution());

	double increment = 1.0 / canvasRadius;
	if (isinf(increment))
	{
		increment = 10;
	} 

	if (filledFlag)
	{
		for (int y = -canvasRadius; y <= canvasRadius; y++)
		{
			for (int x = -canvasRadius; x <= canvasRadius; x++)
			{
				if (x * x + y * y <= canvasRadius * canvasRadius && canvasX + x < bufferWidth - 1 && canvasY + y < bufferHeight - 1)
				{
					buffer[(canvasY + y) * bufferWidth + (canvasX + x)] = rgb;
				}
			}
		}
	}

	else 
	{	
		// If lines are drawn always in white
		rgb = Color(255, 255, 255, "RGB");
		for (double angle = 0; angle < 2 * M_PI; angle += increment)
		{
			x = round(canvasX + canvasRadius * cos(angle));
			y = round(canvasY + canvasRadius * sin(angle));

			if (x < bufferWidth - 1 && y < bufferHeight - 1)
			{
				buffer[y * bufferWidth + x] = rgb;	
			}

		}
	}
}
