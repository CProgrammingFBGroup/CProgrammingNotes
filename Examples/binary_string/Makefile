# Makefile for demo
#
# Written by Matthew Campbell on Saturday March 28, 2020.
#
# Do not define CC if you are using C4droid.
#
# Compiler to use:
#
CC = gcc
#
# Compiler flags:
#
CFLAGS = -c -pedantic -std=c17 -Wall
#
# Define the header file.
#
INC = binary_string.h
#
# Define the source files.
#
SRC = demo.c binary_string.c
#
# Define the object files.
#
OBJ = demo.o binary_string.o
#
# Define the default target.
#
all: demo
#
# Define the demo target.
#
demo: objects
	@echo
	@echo "Linking object files with library files."
	$(CC) $(OBJ) -o demo
	@echo
#
# Define the objects target.
#
objects: $(SRC) $(INC)
	@echo
	@echo "Compiling the source code files."
	$(CC) $(CFLAGS) $(SRC)
#
# Define the clean target.
#
clean:
	@echo "Cleaning up."
	rm -f *.o *.obj demo
#
# EOF
