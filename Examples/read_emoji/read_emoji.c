/*

     read_emoji.c

     Reads a Unicode UTF-8 emoji pictogram from user input.

     Trying to enter something other than a 32-bit Unicode
     UTF-8 emoji pictogram may produce unpredictable results.

     If this does not work for you then it could be a keyboard
     compatibility issue.  I had to copy the emoji to my clipboard
     and paste it when prompted just to get it to work.

     Written by Matthew Campbell.

*/

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main( void )
{
     int exit_loop, pos, ret, save_errno;
     uint32_t emoji;
     uint8_t str[ 6 ];

     exit_loop = 0;
     while( exit_loop == 0 )
     {
          printf( "Please type an emoji pictogram and press Enter: " );
          fflush( stdout );

          for( pos = 0; pos < 4; )
          {
               errno = 0;
               ret = fgetc( stdin );
               save_errno = errno;
               if ( ret == 0 || ret == EOF )
               {
                    save_errno = errno;
                    printf( "\n\
Something went wrong while reading your input.\n" );
                    if ( save_errno != 0 )
                    {
                         printf( "Error: %s.\n", strerror( save_errno ) );
                    }
                    printf( "\n" );
                    exit( EXIT_FAILURE );
               }
               else if ( ret != 10 && ret != 13 )
               {
                    str[ ( pos++ ) ] = ( uint8_t )ret;
               }

          }    /* for( pos = 0; pos < 4; ) */

          exit_loop = 1;
          str[ 5 ] = 0;

     }    /* while( exit_loop == 0 ) */

     /* Build the emoji.  I did it this way to avoid endian isdues. */

     emoji = ( uint32_t )( ( str[ 0 ] << 24 ) + ( str[ 1 ] << 16 ) +
                           ( str[ 2 ] << 8 ) + str[ 3 ] );

     /* Show the result. */

     printf( "Received: %s using 0x%08X.\n\n", str, emoji );

     exit( EXIT_SUCCESS );
}

/* EOF read_emoji.c */
