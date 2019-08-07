/*

     capitalize.c
     Capitalizes the first letter of the string,
     even if it isn't the first byte in the string.
     Written by Matthew Campbell on Thursday January 10, 2019.

*/

#ifndef _CAPITALIZE_C
#define _CAPITALIZE_C

#include <errno.h>

#ifndef EINVAL
#define EINVAL 22
#endif

/*

     This function capitalizes the first letter in the string,
     even if it isn't the first byte in the string.  It won't
     do anything if the string is empty or it doesn't have any
     lower case letters.

     Returns 0 for success or -1 if an error occurs.

*/

int capitalize( char *str )
{
     int done, pos;

     if ( str == NULL )
     {
          errno = EINVAL;
          return 1;
     }
     if ( str[ 0 ] == 0 )
     {
          return 0;  /* Nothing to do. */
     }
     done = pos = 0;
     while( done == 0 && str[ pos ] != 0 )
     {
          if ( str[ pos ] >= 'A' && str[ pos ] <= 'Z' )
          {
               done = 1;
          }
          else if ( str[ pos ] >= 'a' && str[ pos ] <= 'z' )
          {
               str[ pos ] = str[ pos ] - 32;
               done = 1;
          }
          pos++;
     }
     return 0;  /* All done. */
}

#endif  /* _CAPITALIZE_C */

/* EOF capitalize.c */
