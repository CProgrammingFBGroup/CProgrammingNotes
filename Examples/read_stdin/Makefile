# Makefile for read_stdin_demo
#
# Written by Matthew Campbell on Thursday June 13, 2019.
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
# Linker flags:
#
LFLAGS = -lm
#
# Define the source files.
#
SRC = read_stdin_demo.c read_stdin.c
#
# Define the object files.
#
OBJ = read_stdin_demo.o read_stdin.o
#
# Define the default target.
#
all: read_stdin_demo
#
# Define the program target.
#
read_stdin_demo: objects
	@echo
	@echo "Linking object files with library files."
	$(CC) $(LFLAGS) $(OBJ) -o read_stdin_demo
	@echo
#
# Define the objects target.
#
objects: $(SRC)
	@echo
	@echo "Compiling the source code files."
	$(CC) $(CFLAGS) $(SRC)
#
# Define the clean target.
#
clean:
	@echo "Cleaning up."
	rm -f *.o read_stdin_demo
#
# EOF
