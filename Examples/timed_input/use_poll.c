/*

     use_poll.c

     This program uses a poll(3) to
     offer a time limit for user input.

     Written by Matthew Campbell on Friday August 16, 2019.

*/

/* Include what we need: */

#include <poll.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* This is the length of the buffer that holds the user's input. */

#define BUFFER_SIZE 80

int main( void )
{
     char buffer[ BUFFER_SIZE ];
     int answer, ret, save_errno, size;
     struct pollfd fds[ 1 ];

     /* Clear the input buffer. */

     memset( buffer, 0, BUFFER_SIZE );

     /* Clear the pollfd structure. */

     /* This also sets the file stream to stdin. */

     memset( &fds[ 0 ], 0, sizeof( struct pollfd ) );

     /* Tell poll(3) to look for data to read from stdin. */

     fds[ 0 ].events = POLLIN;

     /* Print the question and available answers. */

     printf( "\
You have five seconds to correctly answer this question.\n\n" );

     printf( "What sound does the duck make?\n\n" );
     printf( "1) Meow\n" );
     printf( "2) Woof\n" );
     printf( "3) Quack\n" );
     printf( "4) Moo\n\n" );
     printf( ">> " );
     fflush( stdout );

     /* Use poll(3) to wait up to five seconds. */

     errno = 0;
     ret = poll( fds, 1, 5000 );  /* This is 5,000 milliseconds. */
     if ( ret == ( -1 ) )
     {
          save_errno = errno;
          printf( "\n\nSomething went wrong when calling poll(3).\n" );
          if ( save_errno != 0 )
          {
               printf( "Error: %s.\n", strerror( save_errno ) );
          }
          printf( "\nExiting.\n\n" );
          exit( EXIT_FAILURE );
     }
     else if ( ret == 0 )  /* poll(3) timed out. */
     {
          /*

               Move to the beginning of the line
               and clear to the whole line.

          */

          printf( "%c%c[2K", 13, 27 );

          printf( "\
Time's up.  You did not provide an answer within \
the available time.\n\n" );
          printf( "Exiting.\n\n" );
          exit( EXIT_FAILURE );
     }

     /* Read the input from the stdin buffer. */

     errno = 0;
     if ( fgets( buffer, BUFFER_SIZE, stdin ) == NULL )
     {
          save_errno = errno;
          printf( "Something went wrong while reading your input.\n" );
          if ( save_errno != 0 )
          {
               printf( "Error: %s.\n", strerror( save_errno ) );
          }
          printf( "\nExiting.\n\n" );
          exit( EXIT_FAILURE );
     }

     printf( "\n" );

     /* Did the user only press Enter? */

     if ( buffer[ 0 ] == '\n' )
     {
          printf( "You didn't choose an answer.\n\n" );
          printf( "Exiting.\n\n" );
          exit( EXIT_FAILURE );
     }

     /* Remove the newline character. */

     size = strlen( buffer ) - 1;
     if ( buffer[ size ] == '\n' )
     {
          buffer[ size ] = 0;
     }

     /* Find out if the user provided a useful response. */

     if ( sscanf( buffer, "%d", &answer ) != 1 )
     {
          printf( "That is not valid input.\n\n" );
          printf( "Exiting.\n\n" );
          exit( EXIT_FAILURE );
     }

     if ( answer < 1 || answer > 4 )
     {
          printf( "That is not an available choice.\n\n" );
          printf( "Exiting.\n\n" );
          exit( EXIT_FAILURE );
     }

     /* Did the user provide the correct answer? */

     if ( answer == 3 )
     {
          printf( "Your answer is correct.\n\n" );
          printf( "Successful exit.\n\n" );
          exit( EXIT_SUCCESS );
     }
     else
     {
          printf( "You chose the wrong answer.\n\n" );
          printf( "Exiting.\n\n" );
          exit( EXIT_FAILURE );
     }

     /* And we're done. */
}

/* EOF use_poll.c */
