/* Collection of colorvalues and their respective parameter t \in between 0 and 9 for interpolation with cubic splines TODO: add more colormaps*/

#ifndef COLORMAP_H
#define COLORMAP_H

#include "Colors.h"

const Color DEFAULT_COLORS[6] = {Color(0, 7, 100, "RGB"), Color(32, 107, 203, "RGB"), Color(237, 255, 255, "RGB"), Color(255, 170, 0, "RGB"), Color(0, 2, 0, "RGB"), Color(0, 0, 0, "RGB")};
const double DEFAULT_POINTS[6] = {0, 0.16, 0.42, 0.6425, 0.8575, 1};

const Color GREENS[6] = {Color(0, 102, 51, "RGB"), Color(102, 204, 0, "RGB"), Color(187, 255, 102, "RGB"), Color(0, 255, 128, "RGB"), Color(0, 51, 25, "RGB"), Color(0, 0, 0, "RGB")};
const double GREEN_POINTS[6] = {0, 0.17, 0.33, 0.67, 0.83, 1};

const Color RED_ORANGE[6] = {Color(102, 0, 0, "RGB"), Color(153, 0, 0, "RGB"), Color(255, 0, 0, "RGB"), Color(255, 128, 0, "RGB"), Color(255, 255, 0, "RGB"), Color(255, 255, 255, "RGB")};
const double RED_ORANGE_POINTS[6] = {0, 0.17, 0.33, 0.67, 0.83, 1};

const Color BLUE_PINK_BROWN[6] = {Color(0, 70, 0, "RGB"), Color(0, 153, 0, "RGB"), Color(255, 255, 0, "RGB"), Color(255, 69, 0, "RGB"), Color(153, 0, 0, "RGB"), Color(0, 0, 0, "RGB")};
const double BLUE_PINK_BROWN_POINTS[6] = {0, 0.17, 0.33, 0.67, 0.83, 1};



const Color RED[5] = {Color(255, 0, 0, "RGB"), Color(255, 69, 0, "RGB"), Color(255, 99, 71, "RGB"), Color(139, 0, 0, "RGB"), Color(220, 20, 60, "RGB")};
const Color GREEN[5] = {Color(0, 255, 0, "RGB"), Color(50, 205, 50, "RGB"), Color(0, 128, 0, "RGB"), Color(85, 107, 47, "RGB"), Color(0, 100, 0, "RGB")};
const Color BLUE[5] = {Color(0, 0, 255,"RGB"), Color(30, 144, 255, "RGB"), Color(0, 0, 128, "RGB"), Color(0, 128, 128, "RGB"), Color(70, 130, 180, "RGB")};
const Color YELLOW[5] = {Color(255, 255, 0, "RGB"), Color(240, 230, 140, "RGB"), Color(238, 232, 170, "RGB"), Color(218, 165, 32, "RGB"), Color(255, 215, 0, "RGB")};
const Color MAGENTA[5] = {Color(139, 0, 139, "RGB"), Color(255, 0, 255, "RGB"), Color(238, 130, 238, "RGB"), Color(75, 0, 130, "RGB"), Color(138, 43, 226, "RGB")};


#endif