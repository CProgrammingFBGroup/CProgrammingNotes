/*

     strrev.c
     Reverses the order of the bytes in a character string.
     Written by Matthew Campbell on Saturday November 10, 2018.

*/

#ifndef _STRREV_C
#define _STRREV_C

#include <errno.h>
#include <stdlib.h>
#include <string.h>

char *strrev( const char *str )
{
     char *ret;
     int pos1, pos2, size;

     /* Make sure we have something we can use. */

     if ( str == NULL )
     {
          errno = EINVAL;  /* Invalid parameter */
          return NULL;
     }
     if ( str[ 0 ] == 0 )
     {
          errno = EINVAL;  /* Invalid parameter */
          return NULL;
     }

     /* Make the new string. */

     size = strlen( str ) + 1;  /* Add an extra byte for the null byte. */
     ret = malloc( size * sizeof( char ) );

     if ( ret == NULL )
     {
          errno = ENOMEM;  /* Not enough memory */
          return NULL;
     }

     /* The index starts at zero and we don't want the null byte first. */

     size = size - 2;

     /* Copy everything in the reverse order. */

     for( pos1 = size, pos2 = 0; pos1 >= 0; pos1--, pos2++ )
     {
          ret[ pos2 ] = str[ pos1 ];
     }

     ret[ pos2 ] = 0;  /* Add the null byte to the end of the string. */
     return ret;  /* And we're done. */
}

#endif  /* _STRREV_C */

/* EOF strrev.c */
