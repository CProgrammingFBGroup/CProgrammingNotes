/*

     test_strcmp.c
     Shows how strcmp(3) works.

     Make changes to the strings to see what happens.

     Written by Matthew Campbell on Tuesday January 29, 2019.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* How many bytes long should the strings be? */

#define LENGTH 80

/* Don't use strings longer than LENGTH - 1. */
/* Leave room for the NULL byte.             */

/* Define the contents of the first string. */

#define STR1 "abcde"

/* Define the contents of the second string. */

#define STR2 "abcde"

/* Define main(). */

int main( void )
{
     char str1[ LENGTH ], str2[ LENGTH ];
     int ret, size1, size2;

     /* Find out how many bytes each string is using. */

     size1 = strlen( STR1 );
     size2 = strlen( STR2 );

     /* Make sure we don't overflow the buffer space. */

     /* We use >= LENGTH because the size may be up to LENGTH - 1. */

     if ( size1 >= LENGTH || size2 >= LENGTH )
     {
          printf( "Error: String length exceeds buffer length.\n\n" );
          exit( EXIT_FAILURE );
     }

     /* Write the contents of each string into the correct variables. */

     strcpy( str1, STR1 );
     strcpy( str2, STR2 );

     /* Compare the strings. */

     ret = strcmp( str1, str2 );

     /* Print the results. */

     printf( "str1: \"%s\", strlen( str1 ): %d\n", str1, size1 );
     printf( "str2: \"%s\", strlen( str2 ): %d\n\n", str2, size2 );
     printf( "strcmp( str1, str2 ): %d\n\n", ret );

     /* And we're done. */

     exit( EXIT_SUCCESS );
}

/* EOF test_strcmp.c */
