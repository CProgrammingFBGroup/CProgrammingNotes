# Example Makefile
#
# Written by Matthew Campbell on Tuesday March 12, 2019.
#
# Do not define CC if you are using C4droid.
#
# Compiler to use:
#
#CC = gcc
#
# Compiler flags:
#
CFLAGS = -c -pedantic -std=c11 -Wall
#
# Linker flags:
#
LFLAGS = -lm
#
# Define the source file.
#
SRC = example.c
#
# Define the object file.
#
OBJ = example.o
#
# Define the default target.
#
all: example
#
# Define the example target.
#
example: objects
	@echo "Linking object file with library files."
	$(CC) $(LFLAGS) $(OBJ) -o example
#
# Define the objects target.
#
objects: $(SRC)
	@echo "Compiling the source code file."
	$(CC) $(CFLAGS) $(SRC)
#
# Define the clean target.
#
clean:
	@echo "Cleaning up."
	rm -f *.o example
#
# Define the install target.
#
#install: all
#	@echo "Installing the executable binary file."
#	mv example /data/data/com.n0n3m4.droidc/files/temp
#
# Other information about how to write and use a Makefile may be
# found at https://opensource.com/article/18/8/what-how-makefile
#
# EOF
