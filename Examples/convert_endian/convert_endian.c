/*

     convert_endian.c
     Detects and converts the endianess of a variable.
     Written by Matthew Campbell on Thursday February 28, 2019.

*/

#ifndef _CONVERT_ENDIAN_C
#define _CONVERT_ENDIAN_C

#include <errno.h>
#include <stdio.h>

/*

     This function determines if the architecture in use uses big
     or little endian storage.  Returns 2 for big endian, 1 for
     little endian, and 0 for an unknown endian type.

*/

int detect_endian( void )
{
     union
     {
          char bytes[ 4 ];
          uint32_t num;
     } var;

     var.num = 0x01020408;

     if ( var.bytes[ 0 ] == 1 && var.bytes[ 1 ] == 2 &&
          var.bytes[ 2 ] == 4 && var.bytes[ 3 ] == 8 )
     {
          return 2;  /* Big endian */
     }

     if ( var.bytes[ 0 ] == 8 && var.bytes[ 1 ] == 4 &&
          var.bytes[ 2 ] == 2 && var.bytes[ 3 ] == 1 )
     {
          return 1;  /* Little endian */
     }

     return 0;  /* Unknown */
}

/*

     This function inverts the data structure's data space.
     Returns 0 for success or -1 if an error occurs.

*/

int invert_endian( void *buffer, int size )
{
     char *ptr, tmp;
     int limit, pos1, pos2;

     if ( buffer == NULL || size < 2 )
     {
          errno = EINVAL;
          return ( -1 );  /* Return failure. */
     }
     ptr = ( char * )buffer;
     limit = size / 2;
     for( pos1 = 0, pos2 = ( size - 1 ); pos1 < limit; pos1++, pos2-- )
     {
          tmp = ptr[ pos1 ];
          ptr[ pos1 ] = ptr[ pos2 ];
          ptr[ pos2 ] = tmp;
     }
     return 0;
}

#endif

/* EOF convert_endian.c */