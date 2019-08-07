# Makefile for read_text_demo.c
#
# Written by Matthew Campbell on Tuesday May 21, 2019.
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
SRC = read_text_demo.c read_text.c
#
# Define the object files.
#
OBJ = read_text_demo.o read_text.o
#
# Define the default target.
#
all: read_text_demo
#
# Define the read_text_demo target.
#
read_text_demo: objects
	@echo
	@echo "Linking object files with library files."
	$(CC) $(LFLAGS) $(OBJ) -o read_text_demo
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
	rm -f *.o read_text_demo
#
# EOF
