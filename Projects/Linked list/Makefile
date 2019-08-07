# Makefile for dlink_demo
#
# Written by Matthew Campbell on Wednesday June 12, 2019.
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
# Define the header file.
#
INC = dlink_list.h
#
# Define the source files.
#
SRC = dlink_demo.c dlink_list.c read_stdin.c
#
# Define the object files.
#
OBJ = dlink_demo.o dlink_list.o read_stdin.o
#
# Define the default target.
#
all: dlink_demo
#
# Define the program target.
#
dlink_demo: objects
	@echo
	@echo "Linking object files with library files."
	$(CC) $(LFLAGS) $(OBJ) -o dlink_demo
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
	rm -f *.o dlink_demo
#
# EOF
