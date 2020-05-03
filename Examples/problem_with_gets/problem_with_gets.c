/*

     problem_with_gets.c
     Written by Matthew Campbell.

*/

#include <stdio.h>
#include <stdlib.h>

/* The dumb compiler I had to use to do this didn't have this. */

typedef unsigned int uint32_t;

int main( void )
{
     int num1;
     char array[ 4 ], buffer[ 8 ];
     int num2;

     strcpy( array, "Hi." );

     printf( "Please enter an integer number: " );
     fflush( stdout );

     if ( scanf( "%d", &num1 ) != 1 )
     {
          printf( "\nThat's not an integer number.\n" );
          return 1;
     }
     getchar();  /* Remove the stray newline in the stdin buffer. */

     printf( "\nYou entered: %d.\n\n", num1 );

     printf( "Please enter a second integer number: " );
     fflush( stdout );

     if ( scanf( "%d", &num2 ) != 1 )
     {
          printf( "\nThat's not an integer number.\n" );
          return 1;
     }
     getchar();  /* Remove the stray newline in the stdin buffer. */

     printf( "\nYou entered: %d.\n\n", num2 );

     printf( "Please enter a character string: " );
     fflush( stdout );

     /* Showing why using gets(3) is dangerous. */

     gets( buffer );

     printf( "\narray now holds: \"%s\"\n\n", array );

     printf( "buffer now holds: \"%s\"\n\n", buffer );

     /* But what happens to num1 and num2 if the user offers too much data? */

     printf( "num1 now holds: %d and num2 now holds: %d.\n", num1, num2 );

     printf( "\n\
&num1: %08X, &num2: %08X, &array[ 0 ]: %08X,\n\
&array[ 2 ]: %08X, &buffer[ 0 ]: %08X, &buffer[ 7 ]: %08X\n",
             ( uint32_t )( &num1 ), ( uint32_t )( &num2 ),
             ( uint32_t )( &array[ 0 ] ), ( uint32_t )( &array[ 2 ] ),
             ( uint32_t )( &buffer[ 0 ] ), ( uint32_t )( &buffer[ 7 ] ) );

     return 0;
}

/* EOF problem_with_gets.c */
