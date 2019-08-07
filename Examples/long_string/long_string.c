/*

     long_string.c
     Reads a string of unknown size from stdin.
     Written by Matthew Campbell on Sunday November 18, 2018.

*/

#include <stdio.h>
#include <stdlib.h>

#define BLOCK_SIZE 8

int main( void )
{
     char ch, *ptr, *str;
     int blocks, length;

     /* First we will need some space to store our data. */

     str = malloc( BLOCK_SIZE );

     /* Make sure we received something we can use. */

     if ( str == NULL )
     {
          printf( "Failed to allocate memory.\n" );
          exit( EXIT_FAILURE );
     }

     /* Initialize some variables. */

     blocks = 1;
     length = 0;
     str[ 0 ] = 0;

     /* Print the prompt. */

     printf( "Please enter a string: " );

     /* Use fflush() here since the prompt doesn't end with '\n'. */

     fflush( stdout );

     /* Start the loop to read a character at a time until '\n'. */

     do
     {
          /* Get a character from the stdin buffer. */

          ch = ( char )fgetc( stdin );

          /* Exit if something went wrong. */

          if ( feof( stdin ) || ferror( stdin ) )
          {
               printf( "\nSomething went wrong while reading.\n" );
               exit( EXIT_FAILURE );
          }

          /* Add more space in the buffer if it's full. */

          if ( length == ( blocks * BLOCK_SIZE ) )
          {
               ptr = realloc( str, ( ( blocks + 1 ) * BLOCK_SIZE ) );

               /* Make sure it worked before trying to use it. */

               if ( ptr == NULL )
               {
                    printf( "\nFailed to allocate more memory.\n" );
                    exit( EXIT_FAILURE );
               }

               /* We added another block of memory to the buffer. */

               blocks++;

               /* The new memory block might have a different address. */

               str = ptr;
               ptr = NULL;
          }

          /* Don't store the newline character. */

          if ( ch != '\n' )
          {
               str[ length ] = ch;
               length++;
          }

          /* Are we done reading? */

     }    while( ch != '\n' );

     /* Display the results, */

     printf( "\nYou entered: \"%s\"\n\n", str );

     /* Free the buffer memory. */

     free( str );
     str = NULL;

     /* And we're done. */

     exit( EXIT_SUCCESS );
}

/* EOF long_string.c */
