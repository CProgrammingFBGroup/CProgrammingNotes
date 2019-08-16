# Makefile for timed_input
#
# Written by Matthew Campbell on Friday August 16, 2019.
#
# Do not define CC if you are using C4droid.
#
# Compiler to use:
#
CC = gcc
#
# Compiler flags:
#
# Use the CFLAGS with -pthread if you use use_pthread.c
#
CFLAGS = -c -pedantic -std=c17 -Wall
#CFLAGS = -c -pedantic -pthread -std=c17 -Wall
#
# Define the source file.
#
# Define one and only one of these
# according to what your system requires.
#
#SRC = use_clone.c
#SRC = use_poll.c
#SRC = use_pthread.c
#
# Define the object file.
#
# Do the same here.
#
#OBJ = use_clone.o
#OBJ = use_clone.obj
#OBJ = use_poll.o
#OBJ = use_poll.obj
#OBJ = use_pthread.o
#OBJ = use_pthread.obj
#
# Define the default target.
#
all: timed_input
#
# Define the program target.
#
timed_input: objects
	@echo
	@echo "Linking object file with library files."
	$(CC) $(OBJ) -o timed_input
	@echo
#
# Define the objects target.
#
objects: $(SRC)
	@echo
	@echo "Compiling the source code file."
	$(CC) $(CFLAGS) $(SRC)
#
# Define the clean target.
#
clean:
	@echo "Cleaning up."
	rm -f *.o *.obj timed_input
#
# EOF
