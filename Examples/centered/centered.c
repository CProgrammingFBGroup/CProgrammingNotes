/*

     centered.c
     Prints a string in the center of a line using a simple macro.
     Written by Matthew Campbell on Thursday November 28, 2018.

*/

#include <stdio.h>
#include <string.h>

/* This is the macro. */

#define print_center( x, y, z ) \
printf( "%*s\n", ( ( z - y ) / 2 + y ), x );

int main( void )
{
     char str[] = "This is a string.";
     int cols, len;

     /* Set cols to the number of columns on your display. */

     cols = 80;
     len = strlen( str );

     /* Calling the macro looks just like calling a function. */

     print_center( str, len, cols );
     return 0;
}

/* EOF centered.c */