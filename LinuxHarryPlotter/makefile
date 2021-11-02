
CC = g++

#If openmp is no installed simply remove the flag it should work just fine without it.
CFLAGS = -Ofast -fopenmp -std=c++2a -msse3 -lstdc++

INCLUDES = -Iinclude

#Do not ever change the name of this directory
TARGET_DIR = HarryPlotter

all: src/main.cc

	@echo "Compiling with $(CFLAGS)"
	@$(CC) $(CFLAGS) $(INCLUDES) src/* -o HarryPlotterExec
	
	@echo "Compiled successfully!"
	@mkdir ~/$(TARGET_DIR)

	@echo "Created main directory!"
	@mkdir ~/$(TARGET_DIR)/Results

	@cp HarryPlotterExec ~/$(TARGET_DIR)
	@echo "Created results directory!"
	@echo "Installation completed"
clean:
	$(RM) HarryPotterExec
