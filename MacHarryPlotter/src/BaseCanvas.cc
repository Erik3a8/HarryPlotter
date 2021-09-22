#include <fstream>
#include <cstdio>




double BaseCanvas::resolution {0.001};
unsigned int BaseCanvas::height {1000};
unsigned int BaseCanvas::width {1000};
double BaseCanvas::centerX {0.0};
double BaseCanvas::centerY {0.0};
std::string BaseCanvas::PATH {""};

// Access methods
unsigned int BaseCanvas::getHeight ()
{
	return height;
}

unsigned int BaseCanvas::getWidth ()
{
	return width;
}

double BaseCanvas::getResolution ()
{
	return resolution;
}

double BaseCanvas::getCenterX ()
{
	return centerX;
}

double BaseCanvas::getCenterY ()
{
	return centerY;
}

Color* BaseCanvas::getBuffer ()
{
	return this->buffer;
}

Color BaseCanvas::getPixel (unsigned int x, unsigned int y) 
{
	return buffer[y * width + x];
}

std::string BaseCanvas::getPath()
{   
    return PATH;
}

// modifier methods
void BaseCanvas::setHeight (unsigned int h)
{
    height = h;
}

void BaseCanvas::setWidth (unsigned int w)
{
    width = w;
}

void BaseCanvas::setResolution (double res)
{
    resolution = res;
}

void BaseCanvas::setCenterX (double CX)
{
    centerX = CX;
}

void BaseCanvas::setCenterY (double CY)
{
    centerY = CY;
}


void BaseCanvas::resetBuffer ()
{
    Color black(0,0,0, "RGB");
    for (int i = 0; i < getWidth(); i++)
    {
        for (int j = 0; j < getHeight(); j++)
        {
            this->setPixel(i, j, black);
        }
    }
}

void BaseCanvas::setPixel(unsigned int x, unsigned int y, const Color Pixel)
{
    buffer[y * width + x] = Pixel;
}

void BaseCanvas::setPath(std::string path)
{
    PATH = path;
}


// virtual methods
void BaseCanvas::populatePixel (const int x, const int y, Function &F) {}
void BaseCanvas::populateCanvas (Function &F, const int interval) {}



void BaseCanvas::saveBMP (const char* filename) 
{
    std::string OUTPATH = PATH + "/" + filename;
    int WIDTH_IN_BYTES = width * BYTES_PER_PIXEL;

    unsigned char padding[3] = {0, 0, 0};
    int paddingSize = (4 - (WIDTH_IN_BYTES) % 4) % 4;

    int stride = (WIDTH_IN_BYTES) + paddingSize;

    FILE* imageFile = fopen(OUTPATH.c_str(), "wb");

    unsigned char* fileHeader = createBitmapFileHeader(stride);
    fwrite(fileHeader, 1, FILE_HEADER_SIZE, imageFile);

    unsigned char* infoHeader = createBitmapInfoHeader();
    fwrite(infoHeader, 1, INFO_HEADER_SIZE, imageFile);


    for (int y = height - 1; y >= 0; y--) 
    {
        for (int x = 0; x < width; x++) 
        {	
            // Get the Color values at this Pixel
            Color Pixel = getPixel(x, y);

            // Convert to char 
            unsigned char R = Pixel.getFirst();
            unsigned char G = Pixel.getSecond();
            unsigned char B = Pixel.getThird();

            // Write to file
            fwrite(&B, 1, 1, imageFile);
            fwrite(&G, 1, 1, imageFile);
            fwrite(&R, 1, 1, imageFile);
        }

        // After end of line write the padding
        fwrite(padding, 1, paddingSize, imageFile);
    }

    fclose(imageFile);
}

unsigned char* BaseCanvas::createBitmapFileHeader (int stride)
{
    int fileSize = FILE_HEADER_SIZE + INFO_HEADER_SIZE + (stride * height);

    static unsigned char fileHeader[] = {
        0,0,     /// signature
        0,0,0,0, /// image file size in bytes
        0,0,0,0, /// reserved
        0,0,0,0, /// start of pixel array
    };

    fileHeader[ 0] = (unsigned char)('B');
    fileHeader[ 1] = (unsigned char)('M');
    fileHeader[ 2] = (unsigned char)(fileSize      );
    fileHeader[ 3] = (unsigned char)(fileSize >>  8);
    fileHeader[ 4] = (unsigned char)(fileSize >> 16);
    fileHeader[ 5] = (unsigned char)(fileSize >> 24);
    fileHeader[10] = (unsigned char)(FILE_HEADER_SIZE + INFO_HEADER_SIZE);

    return fileHeader;
}

unsigned char* BaseCanvas::createBitmapInfoHeader ()
{
    static unsigned char infoHeader[] = {
        0,0,0,0, /// header size
        0,0,0,0, /// image width
        0,0,0,0, /// image height
        0,0,     /// number of color planes
        0,0,     /// bits per pixel
        0,0,0,0, /// compression
        0,0,0,0, /// image size
        0,0,0,0, /// horizontal resolution
        0,0,0,0, /// vertical resolution
        0,0,0,0, /// colors in color table
        0,0,0,0, /// important color count
    };

    infoHeader[ 0] = (unsigned char)(INFO_HEADER_SIZE);
    infoHeader[ 4] = (unsigned char)(width      );
    infoHeader[ 5] = (unsigned char)(width >>  8);
    infoHeader[ 6] = (unsigned char)(width >> 16);
    infoHeader[ 7] = (unsigned char)(width >> 24);
    infoHeader[ 8] = (unsigned char)(height      );
    infoHeader[ 9] = (unsigned char)(height >>  8);
    infoHeader[10] = (unsigned char)(height >> 16);
    infoHeader[11] = (unsigned char)(height >> 24);
    infoHeader[12] = (unsigned char)(1);
    infoHeader[14] = (unsigned char)(BYTES_PER_PIXEL * 8);

    return infoHeader;
}
