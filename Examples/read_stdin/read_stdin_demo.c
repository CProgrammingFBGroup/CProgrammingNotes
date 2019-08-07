/*

     read_stdin_demo.c
     This file offers a simple example of how to use read_stdin().
     Written by Matthew Campbell.

*/

#include <errno.h>
#include <stdio.h>
#include <string.h>

/* Prototype read_stdin() */

int read_stdin( char *buffer, const int length, const char *prompt,
                const int reprompt );

int main( void )
{
     char buffer[ 1024 ];
     int ret, save_errno;

     errno = 0;
     ret = read_stdin( buffer, 1024, "prompt >> ", 1 );
     save_errno = errno;
     if ( ret != 0 )
     {
          printf( "\nSomething went wrong." );
          if ( save_errno != 0 )
          {
               printf( "  Error: %s.\n", strerror( save_errno ),
                                              save_errno );
          }
          else
          {
               printf( "\n" );
          }
          return 1;
     }
     printf( "Received: \"%s\"\n", buffer );
     return 0;
}

/* EOF read_stdin_demo.c */
