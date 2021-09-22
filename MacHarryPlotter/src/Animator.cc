
unsigned int Animation::FrameCount{1};
unsigned int Animation::fps{30};

bool Animation::breakoutFlag{false};
bool Animation::zoomFlag{false};

double Animation::zoomConstant{0};

unsigned int Animation::getFrameCount () 		
{
	return FrameCount;
}

unsigned int Animation::getFps () 		   		
{
	return fps;
}

bool Animation::getBreakoutFlag () 
{
	return breakoutFlag;
}

bool Animation::getZoomFlag ()
{
	return zoomFlag;
}

double Animation::getZoomConstant ()
{
	return zoomConstant;
}

void Animation::setFps (const int frames)          		
{
	fps = frames;
}

void Animation::setFrameCount (const int F_C)  		
{
	FrameCount = F_C;
}

void Animation::setBreakoutFlag (const bool B_F)
{
	breakoutFlag = B_F;
}

void Animation::setZoomFlag (const bool Z_F)
{
	zoomFlag = Z_F;
}

void Animation::setZoomConstant (const double constant)
{
	zoomConstant = constant;
}

void Animation::zoom (const int iteration)
{
	// Zooming by modifying the resolution
	this->setResolution(this->getResolution() * (zoomConstant));

	// Modifying the actual "mathematical" size
	realSizeX = getResolution() * getWidth();
	realSizeY = getResolution() * getHeight();
}

void Animation::render (Function &F, const unsigned int interval, const double fade, const char* filename)
{
	std::string OUTPATH = this->getPath() + "/" + filename;
	// Remember the initial resolution
	double initialResolution = getResolution();	
	FILE *fp = fopen(OUTPATH.c_str(), "wb");

	unsigned char *frame = new unsigned char[getWidth() * getHeight() * 4];

	for (int i = 0; i < FrameCount; i++)
	{
		F.setFunctionIterator(i);
		if (breakoutFlag)
		{
			F.setBreakoutIterator(i);
		}

		if (zoomFlag)
		{
			zoom(i);
		}

		this->populateCanvas(F, interval);

		for (int Y = getHeight() - 1; Y >= 0; Y--) 
    	{
        	for (int X = 0; X < getWidth(); X++) 
        	{	
            	// Get the Color values at this Pixel
            	Color Pixel = getPixel(X, Y);

            	// Convert to char 
            	unsigned char R = std::clamp((int)Pixel.getFirst(), 0, UINT8_MAX);
            	unsigned char G = std::clamp((int)Pixel.getSecond(), 0, UINT8_MAX);
            	unsigned char B = std::clamp((int)Pixel.getThird(), 0, UINT8_MAX);

            	// Write to the Frame (NOTE: Little Endian)
            	frame[Y * 4 * getWidth() + 4 * X + 0] = R;
            	frame[Y * 4 * getWidth() + 4 * X + 1] = G;
            	frame[Y * 4 * getWidth() + 4 * X + 2] = B;
            	frame[Y * 4 * getWidth() + 4 * X + 3] = 0;	
        	}
    	}

   		jo_write_mpeg(fp, frame, getWidth(), getHeight(), fps);
   		this->resetBuffer();
	}

	setResolution(initialResolution);	
	fclose(fp);
}


