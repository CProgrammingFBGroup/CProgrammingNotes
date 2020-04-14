/*

     emoji_to_string.c

     Converts a 32-bit Unicode UTF-8 emoji to
     a printable unsigned character string.

     written by Matthew Campbell.

*/

#include <errno.h>
#include <stdio.h>

/*

     This library function converts a 32-bit Unicode emoji pictogram
     into a printable unsigned character string.  The string buffer
     str must be an unsigned character array of at least 5 bytes.

     This function will return a pointer to the unsigned character
     string, or a null pointer if an error occurs.

     This function will set errno as follows:

     EFAULT: str holds a null pointer.

     ENODATA: emoji holds a value of 0.

*/

/*

     A uint32_t variable may be loaded with a Unicode UTF-8 emoji like this:

     uint32_t emoji = 0xF09F9880;

     which is a smiley face emoji pictogram with open eyes.

*/

uint8_t *emoji_to_string( uint32_t emoji, uint8_t *str )
{

     /* Make sure we have parameters that we can use. */

     if ( emoji == 0 )
     {
          errno = ENODATA;
          return NULL;
     }

     if ( str == NULL )
     {
          errno = EFAULT;
          return NULL;
     }

     /* The conversion was done this way to avoid endian issues. */

     str[ 0 ] = ( uint8_t )( ( emoji & 0xFF000000 ) >> 24 );
     str[ 1 ] = ( uint8_t )( ( emoji & 0x00FF0000 ) >> 16 );
     str[ 2 ] = ( uint8_t )( ( emoji & 0x0000FF00 ) >> 8 );
     str[ 3 ] = ( uint8_t )( emoji & 0x000000FF );
     str[ 4 ] = 0;

     /* Return the printable unsigned character string str. */

     return str;
}

/* EOF emoji_to_string.c */
