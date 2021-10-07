/* Abstract class containing several geometric objects like lines, rays, squares, circles... */

#ifndef CONSTRUCTION_H
#define CONSTRUCTION_H

#include "Circle.h"
#include <stack>

class Construction {

public:

	Construction () = default; 
	virtual ~Construction () = default;
	std::stack<Circle> objectQueue;
}; 

#endif