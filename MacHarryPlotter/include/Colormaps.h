/* Collection of colorvalues and their respective parameter t \in between 0 and 9 for interpolation with cubic splines TODO: add more colormaps*/
#include "Colors.h"

#ifndef COLORMAP_H
#define COLORMAP_H

Color DEFAULT_COLORS[6] = {Color(0, 7, 100, "RGB"), Color(32, 107, 203, "RGB"), Color(237, 255, 255, "RGB"), Color(255, 170, 0, "RGB"), Color(0, 2, 0, "RGB"), Color(0, 0, 0, "RGB")};
double DEFAULT_POINTS[6] = {0, 0.16, 0.42, 0.6425, 0.8575, 1};

Color GREENS[6] = {Color(0, 102, 51, "RGB"), Color(102, 204, 0, "RGB"), Color(187, 255, 102, "RGB"), Color(0, 255, 128, "RGB"), Color(0, 51, 25, "RGB"), Color(0, 0, 0, "RGB")};
double GREEN_POINTS[6] = {0, 0.17, 0.33, 0.67, 0.83, 1};

Color RED_ORANGE[6] = {Color(102, 0, 0, "RGB"), Color(153, 0, 0, "RGB"), Color(255, 0, 0, "RGB"), Color(255, 128, 0, "RGB"), Color(255, 255, 0, "RGB"), Color(255, 255, 255, "RGB")};
double RED_ORANGE_POINTS[6] = {0, 0.17, 0.33, 0.67, 0.83, 1};

Color BLUE_PINK_BROWN[6] = {Color(0, 70, 0, "RGB"), Color(0, 153, 0, "RGB"), Color(255, 255, 0, "RGB"), Color(255, 69, 0, "RGB"), Color(153, 0, 0, "RGB"), Color(0, 0, 0, "RGB")};
double BLUE_PINK_BROWN_POINTS[6] = {0, 0.17, 0.33, 0.67, 0.83, 1};

#endif