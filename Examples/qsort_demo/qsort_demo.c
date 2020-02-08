/*

     qsort_demo.c

     Demonstrates the use of the qsort(3) function.
     The qsort(3) function will sort the array in ascending order.
     The example in the man page does not work.

     Written by Matthew Campbell.

*/

/* Include what we need: */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Define REVERSE_SORT_ORDER to sort in descending order. */

#undef REVERSE_SORT_ORDER

/* This is the prototype for the callback function: */

int compare( const void *arg1, const void *arg2 );

int main( void )
{
     /* Define the words array to be sorted: */

     char words[ 5 ][ 6 ] = { "one", "two", "three", "four", "five" };

     /* limit is the size of the first dimension. */
     /* size is the size of the second dimension. */

     int limit = 5, pos, save_errno, size = 6;

     /* Print the words array before sorting it. */

     printf( "Starting with:" );
     for( pos = 0; pos < limit; pos++ )
     {
          printf( " \"%s\"", words[ pos ] );
     }
     printf( "\n" );

     /* Use qsort(3) to sort the words array. */

     errno = 0;
     qsort( words, limit, size, compare );
     save_errno = errno;

     /* See if anything went wrong. */

     if ( save_errno != 0 )
     {
          printf( "qsort(3) or compare() returned an error: %s.\n",
                  strerror( save_errno ) );
     }

     /* Print the words array after sorting it. */

     printf( "Sorted output:" );
     for( pos = 0; pos < limit; pos++ )
     {
          printf( " \"%s\"", words[ pos ] );
     }
     printf( "\n" );

     exit( EXIT_SUCCESS );
}

/* This function compares the two strings in the words array for qsort(3). */

int compare( const void *arg1, const void *arg2 )
{
     char *str1, *str2;
     int ret;

     /* Make sure we have parameters that we can use. */

     if ( arg1 == NULL || arg2 == NULL )
     {
          errno = EFAULT;
          return 0;
     }

     /* Type cast the strings so we can use them. */

     str1 = ( char * )arg1;
     str2 = ( char * )arg2;

     /* Compare the strings. */

     ret = strcmp( str1, str2 );

     /* Use descending order if requested. */

#ifdef REVERSE_SORT_ORDER

     ret = ret * ( -1 );

#endif

     /* Return the result of the comparison. */

     return ret;
}

/* EOF qsort_demo.c */
