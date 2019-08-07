/*

     read_text_demo.c
     This file offers a simple example of how to use read_text().
     Written by Matthew Campbell on Tuesday May 21, 2019.

*/

#include <errno.h>
#include <stdio.h>
#include <string.h>

int main( void )
{
     char buffer[ 1024 ];
     int length, ret, save_errno, size;

     errno = 0;
     ret = read_text( buffer, 1024, stdin, "prompt >> ", 0 );
     save_errno = errno;
     if ( ret == ( -1 ) )
     {
          printf( "Something went wrong." );
          if ( save_errno != 0 )
          {
               printf( "  Error: %s.\n", strerror( errno ) );
          }
          else
          {
               printf( "\n" );
          }
          return 1;
     }
     printf( "Received: \"%s\"\n", buffer );
     printf( "Number of bytes read: %d.\n", ret );
     if ( save_errno != 0 )
     {
          printf( "Error message returned: %s.\n",
                  strerror( save_errno ) );
     }
     return 0;
}

/* EOF read_text_demo.c */
