/*

     lower_case.c
     Converts all letters in a string to lower case.
     Written by Matthew Campbell on Thursday January 10, 2019.

*/

#ifndef _LOWER_CASE_C
#define _LOWER_CASE_C

#include <errno.h>

#ifndef EINVAL
#define EINVAL 22
#endif

/*

     This function converts all upper case letters in a string to
     lower case letters.  It won't do anything if the string is empty.

     Returns 0 for success or -1 if an error occurs.

*/

int lower_case( char *str )
{
     int pos;

     if ( str == NULL )
     {
          errno = EINVAL;
          return 1;
     }
     if ( str[ 0 ] == 0 )
     {
          return 0;  /* Nothing to do. */
     }
     pos = 0;
     while( str[ pos ] != 0 )
     {
          if ( str[ pos ] >= 'A' && str[ pos ] <= 'Z' )
          {
               str[ pos ] = str[ pos ] + 32;
          }
          pos++;
     }
     return 0;  /* All done. */
}

#endif  /* _LOWER_CASE_C */

/* EOF lower_case.c */
