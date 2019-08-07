/*

     upper_case.c
     Converts all letters in a string to upper case.
     Written by Matthew Campbell on Thursday January 10, 2019.

*/

#ifndef _UPPER_CASE_C
#define _UPPER_CASE_C

#include <errno.h>

#ifndef EINVAL
#define EINVAL 22
#endif

/*

     This function converts all lower case letters in a string to
     upper case letters.  It won't do anything if the string is empty.

     Returns 0 for success or -1 if an error occurs.

*/

int upper_case( char *str )
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
          if ( str[ pos ] >= 'a' && str[ pos ] <= 'z' )
          {
               str[ pos ] = str[ pos ] - 32;
          }
          pos++;
     }
     return 0;  /* All done. */
}

#endif  /* _UPPER_CASE_C */

/* EOF upper_case.c */
