# HarryPlotter
 Simple complex valued function plotter. Includes a minimalistic JIT interpreter (macOS only). No dependencies except openmp (optional) and the g++ compiler.

# Description
Plot complex valued functions using domain coloring or grid mapping and output the results as bitmap files or mpg video files. The JIT interpreter for macOS works by creating a DLL and recompiling it for each new entered expression. The Linux version uses the shunting yard algorithm to translate input to reverse polish notation (about 5 times slower than macOS). Also includes the possibility to plot certain fractals using the escape time algorithm. UI is a basic CLI. The Complex number arithmetic was implemented using SSE instructions. 

# Getting Started
Choose the appropriate release for your operating system (macOS or Linux (tested on Ubuntu 20.04)). The main program folder will be located in the home directory. Install by running the makefile. If you do not have openmp installed simply remove the "-fopenmp" flag from the makefile. Run the program either from the shell (Linux and macOS) using "./HarryPlotterExec" or by double clicking the HarryPlotterExec file (macOS only).

# Escape Time Mode
This mode will let you render fractals using the escape time algorithm.
After startup hit "1" and press enter in the main menu. You may choose one of the following fractals by navigating to the Configure menu: 
 1. Mandelbrot Set
 2. Tricorn
 3. Burning Ship
You may also change the center point, the real size (size in the complex plane), the size in pixels (to alter the resolution) and the maximum number of iterations.

# Complex Function Domain Coloring Mode/Grid Mode
This mode lets you plot a complex valued function using the domain coloring method / grid mapping method (either output a .bmp imagefile or a .mpg video file). You may also change the size, center point, space between to gridlines (Grid Mode), framerate and number of frames (for movies) and additionally you can choose the number of iterations to plot Julia sets. For notes on the JIT compiler/Interpreter refer to the next section. 

# Syntax for entering mathematical expressions
The interpreter recognizes many common functions (exp, log, sin, cos, tan, arccos, arcsin, arctan, sinh, cosh, tanh, arsinh, arcosh, artanh). To enter exponentiation put brackets around the expression to be exponentiated. (e.g. (z)^2 and NOT z^2). So far only integer exponents are supported. To enter imaginary numbers write i# where # represents some number. If you write N this indicates to the interpreter that this value should be increased by 1 for each frame (in case of movie rendering) Some examples for valid expressions are:
 1. (z)^2 + z * exp(2 * PI * i1 * 1 / 3)
 2. cosh(z) / sinh(z)
 3. log(4 + i2)
 4. (z)^N

The expressions will be translated to C++ code and then compiled using g++. Some examples of invalid syntax are:
 1. z^3 (no brackets around "z")
 2. (z)^(N + 6) (only whole numbers are allowed in exponents)
 3. 3 + 6i (imaginary numbers have to be entered as i6)
 4. In general expressions that are nonsensical in math

You can also plot Eisenstein series by entering Eis(z, Weight) (Eis4(z), Eis6(z) on Linux) where "Weight" has to be an even integer greater than 2. Furthermore Eisenstein series are only defined in the upper half plane. Note that all other weights can also be constructed from 4 and 6 (see wikipedia). 

# Future Work
Implement more general expressions as exponents. Make the JIT compiler work on Linux and make it more secure. Write/include a better video encoder. Add more colomaps. Add CUDA/Metal/OpenGL for faster rendering and interactive manipulation. Maybe a GUI. Find a better name... 

# Acknowledgements
The .mpg encoder was taken from here: https://www.jonolick.com/home/mpeg-video-writer




