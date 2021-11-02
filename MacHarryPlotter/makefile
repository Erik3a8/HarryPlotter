CC = g++

#If openmp is not installed simply remove the flag it should work just fine without it.
CFLAGS = -Ofast -fopenmp -std=c++2a -msse3 -lstdc++

INCLUDES = -Iinclude

#Do not ever change the name of this directory
TARGET_DIR = HarryPlotter

all: src/main.cc
	@echo "Compiling with: $(CFLAGS)"
	@$(CC) $(CFLAGS) $(INCLUDES) src/* -o HarryPlotterExec

	@echo "Compiled successfully!"
	@mkdir ~/$(TARGET_DIR) 

	@echo "Created main directory!"
	@mkdir ~/$(TARGET_DIR)/Results ~/$(TARGET_DIR)/JITsource

	@cp HarryPlotterExec ~/$(TARGET_DIR)
	@echo "Created results and JITsource directories!"

	@cp include/{Colormaps.h,Colors.h,Complex.h,Eisenstein.h,Function.h,Interpolator.h,jit_compile.h} ~/$(TARGET_DIR)/JITsource
	@cp src/{Colors.cc,Complex.cc,Eisenstein.cc,Function.cc,Interpolator.cc} ~/$(TARGET_DIR)/JITsource

	@echo "Copied source files required for JIT into JITsource folder!"
	@echo "Installation completed"

clean:
	$(RM) HarryPlotterExec
