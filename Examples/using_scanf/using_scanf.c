/*

     using_scanf.c
     Written by Matthew Campbell.

*/

/* Include what we need: */

#include <stdio.h>

/* Define main().  Note that main() returns an int. */

int main( void )
{
     /* Define two integer variables. */

     int num, ret;

     /* Tell the user what we want. */

     printf( "Please enter an integer number: " );

     /*

          Use this to force the buffer to be written out.

          The stdout file stream is line buffered.  It will
          be written to the console or terminal when a newline
          character is found, or every so often.  Using fflush(3)
          forces whatever is in the buffer to be written on
          demand.

     */

     fflush( stdout );

     /* Try to read input from the user. */

     ret = scanf( "%d", &num );

     /* Let's find out what both integer variables hold. */

     printf( "\nnum: %d, ret: %d.\n", num, ret );

     /* We're all done so we return 0. */

     return 0;
}

/* EOF using_scanf.c */
