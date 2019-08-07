# Makefile for files
#
# Written by Matthew Campbell on Tuesday June 18, 2019.
#
# Do not define CC if you are using C4droid.
#
# Compiler to use:
#
#CC = gcc
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
SRC = files.c create_file.c display_stat_structure.c does_file_exist.c read_stdin.c
#
# Define the object files.
#
OBJ = files.o create_file.o display_stat_structure.o does_file_exist.o read_stdin.o
#
# Define the default target.
#
all: files
#
# Define the files target.
#
files: objects
	@echo
	@echo "Linking object files with library files."
	$(CC) $(LFLAGS) $(OBJ) -o files
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
	rm -f *.o files
#
# EOF
