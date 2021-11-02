/* Abstract class containing several geometric objects like lines, rays, squares, circles... */

#ifndef GEO_OBJECT_H
#define GEO_OBJECT_H

#include "Colors.h"

class GeoObject {

public:

	GeoObject() = default; 
	virtual ~GeoObject () = default;
	virtual void draw (Color rgb, Color buffer[], unsigned int bufferWidth, unsigned int bufferHeight) {return;};

	/* It turns out 4 parameters are sufficient to describe either lines or circles. 
	param1 = radius
	param2 = blank
	param3 = center.Re()
	param4 = center.Im()

	param1&param2 = beginning 
	param3&param4 = ending
	*/
	double parameter1, parameter2, parameter3, parameter4;
}; 

#endif