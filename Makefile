# Compiler and compiler flags
CC = gcc
CFLAGS = -Wall -Wextra

# Name of the executable
TARGET = examain

# Source files
SRCS = examain.c

# Object files (generated from source files)
OBJS = $(SRCS:.c=.o)

# Doxygen configuration file
DOXYFILE = Doxyfile

# Default rule
all: $(TARGET)

# Rule to build the executable
$(TARGET): $(OBJS) libExalib
	$(CC) $(CFLAGS) -L. -lExalib -o $@ $<

# Rule to create a static library
libExalib: functionsExamain.o
	ar rcs libExalib.a functionsExamain.o
	
# Rule to compile C source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
	
# Rule to generate Doxygen documentation
doxygen:
	doxygen $(DOXYFILE)

# Clean rule to remove object files and the executable
clean:
	rm -f $(OBJS) $(TARGET)

# Clean rule to remove Doxygen documentation
cleandoxy:
	rm -rf html latex

# Phony targets to avoid conflicts with filenames
.PHONY: all clean doxygen cleandoxy

