/* Create a .mpg file but calculate each frame as we go along to release pressure from the RAM. 
also includes the possibility to include some sort of iterator variable. Note: inherit from BaseCanvas class
this is logically not very pretty (technically this is not a Canvas) but makes the code a lot easier to write.
*/

#ifndef ANIMATOR_H
#define ANIMATOR_H

#include "BaseCanvas.h"

class Animation : public BaseCanvas {
private: 

	static unsigned int FrameCount;

	// Frames per Second (Only 24, 25, 30, 50, or 60)
	static unsigned int fps;

	// Determine whether the max number of iterations is increased
	static bool breakoutFlag;

	// Determine whether we zoom in with each step
	static bool zoomFlag;

	// by how much do we zoom in each step
	static double zoomConstant;

public:

	// Access to Private Members
	static unsigned int getFrameCount ();
	static unsigned int getFps ();
	static bool getBreakoutFlag ();
	static bool getZoomFlag ();
	static double getZoomConstant ();

	// Modify Private Members
	static void setFrameCount (const int numberOfFrames);
	static void setFps (const int fps);
	static void setBreakoutFlag (const bool B_O);
	static void setZoomFlag (const bool Z_F);
	static void setZoomConstant (const double zoom);

	// Render and do not clear the Buffer, Pixels are added frame to frame (expcept when fade is set to 0) 
	void render (Function &F, const unsigned int interval, const double fade, const char* filename);

	void zoom (const int iteration);

};



#endif