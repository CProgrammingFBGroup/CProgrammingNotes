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

/* Define DEBUG to enable debugging output. */

#undef DEBUG

/* Define REVERSE_SORT_ORDER to sort in descending order. */

#undef REVERSE_SORT_ORDER

/* Define USE_POINTERS to use an array of character pointers. */

#undef USE_POINTERS

/* This is the prototype for the callback function: */

int compare( const void *arg1, const void *arg2 );

int main( void )
{

#ifdef USE_POINTERS

     char **words;
     int pos2;

#else

     /* Define the words array to be sorted: */

     char words[ 5 ][ 6 ] = { "one", "two", "three", "four", "five" };

#endif  /* USE_POINTERS */

     /* limit is the size of the first dimension. */
     /* size is the size of the second dimension. */

     int limit = 5, pos, save_errno, size = 6;

     /* Set up the array of character pointers. */

#ifdef USE_POINTERS

          /* Allocate memory for the array of pointers. */

          errno = 0;
          words = malloc( limit * sizeof( char * ) );
          save_errno = errno;

          /* See if that worked. */

          if ( words == NULL )
          {
               printf( "\
Failed to allocate memory for the array of pointers.\n" );
               if ( save_errno != 0 )
               {
                    printf( "Error: %s.n", strerror( save_errno ) );
               }
               exit( EXIT_FAILURE );
          }

          /* Create a dynamically allocated character array for each spot. */

          for( pos = 0; pos < limit; pos++ )
          {
               errno = 0;
               words[ pos ] = malloc( size * sizeof( char ) );
               save_errno = errno;

               /* See if that worked. */

               if ( words[ pos ] == NULL )
               {
                    printf( "\
Failed to allocate memory for string position %d.\n", pos );
                    if ( save_errno != 0 )
                    {
                         printf( "Error: %s.\n", strerror( save_errno ) );
                    }

                    /* Free the array of character pointers. */

                    for( pos2 = 0; pos2 < pos; pos2++ )
                    {
                         free( words[ pos2 ] );
                         words[ pos2 ] = NULL;
                    }
                    free( words );
                    words = NULL;

                    exit( EXIT_FAILURE );
               }

          }    /* for( pos = 0; pos < limit; pos++ ) */

          /* Write the words into their respective character arrays. */

          strcpy( words[ 0 ], "one" );
          strcpy( words[ 1 ], "two" );
          strcpy( words[ 2 ], "three" );
          strcpy( words[ 3 ], "four" );
          strcpy( words[ 4 ], "five" );
          
#endif  /* USE_POINTERS */

     /* Print the words array before sorting it. */

     printf( "Starting with:" );
     for( pos = 0; pos < limit; pos++ )
     {
          printf( " \"%s\"", words[ pos ] );
     }
     printf( "\n" );

     /* Use qsort(3) to sort the words array. */

#ifdef USE_POINTERS

     errno = 0;
     qsort( words, limit, sizeof( char * ), compare );
     save_errno = errno;

#else

     errno = 0;
     qsort( words, limit, size, compare );
     save_errno = errno;

#endif  /* USE_POINTERS */

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

     /* Free the array of character pointers. */

#ifdef USE_POINTERS

     for( pos = 0; pos < limit; pos++ )
     {
          free( words[ pos ] );
          words[ pos ] = NULL;
     }
     free( words );
     words = NULL;

#endif  /* USE_POINTERS */

     exit( EXIT_SUCCESS );
}

/* This function compares the two strings in the words array for qsort(3). */

int compare( const void *arg1, const void *arg2 )
{

#ifdef USE_POINTERS

     char **ptr;

#endif

     char *str1, *str2;
     int ret;

     /* Make sure we have parameters that we can use. */

     if ( arg1 == NULL || arg2 == NULL )
     {
          errno = EFAULT;
          return 0;
     }

     /* Type cast the strings so we can use them. */

#ifdef USE_POINTERS

     ptr = ( char ** )arg1;
     str1 = *ptr;
     ptr = ( char ** )arg2;
     str2 = *ptr;

#else

     str1 = ( char * )arg1;
     str2 = ( char * )arg2;

#endif  /* USE_POINTERS */

     /* Compare the strings. */

#ifdef DEBUG

     printf( "Comparing \"%s\" to \"%s\".\n", str1, str2 );

#endif

     ret = strcmp( str1, str2 );

#ifdef DEBUG

    printf( "ret: %d.\n", ret );

#endif

     /* Use descending order if requested. */

#ifdef REVERSE_SORT_ORDER

     ret = ret * ( -1 );

#endif

     /* Return the result of the comparison. */

     return ret;
}

/* EOF qsort_demo.c */
