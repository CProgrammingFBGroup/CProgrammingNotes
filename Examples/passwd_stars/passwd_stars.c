/*

     passwd_stars.c

     Shows how to read a password while showing stars instead of text.

     Written by Matthew Campbell on Thursday August 16, 2018.

*/

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <termios.h>

/* read_stdin() needs these... */

#ifndef EFAULT
#define EFAULT 14
#endif

#ifndef EINVAL
#define EINVAL 22
#endif

#ifndef ENOMSG
#define ENOMSG 42
#endif

int read_stdin( char *buffer, const int length, const char *prompt );

int main( void )
{
     char ch, pass[ 128 ], user[ 128 ];
     int pos, ret;
     struct termios data;

     /* Read the terminal attributes. */

     ret = tcgetattr( 0, &data );
     if ( ret != 0 )
     {
          ret = errno;
          printf( "Something went wrong. (1)" );
          if ( ret != 0 )
          {
               printf( "  Error: %s.\n", strerror( ret ) );
          }
          else
          {
               printf( "\n" );
          }
          exit( EXIT_FAILURE );
     }

     /* Ask for the user name. */

     ret = read_stdin( user, 128, "login: " );
     if ( ret != 0 )
     {
          ret = errno;
          printf( "Something went wrong. (2)" );
          if ( ret != 0 )
          {
               printf( "  Error: %s.\n", strerror( ret ) );
          }
          else
          {
               printf( "\n" );
          }
          exit( EXIT_FAILURE );
     }

     /* Turn ECHO and ICANON off. */

     data.c_lflag = ISIG | ECHOE | ECHOK | ECHONL | IEXTEN;
     ret = tcsetattr( 0, TCSANOW, &data );
     if ( ret != 0 )
     {
          ret = errno;
          printf( "Something went wrong. (3)" );
          if ( ret != 0 )
          {
               printf( "  Error: %s.\n", strerror( ret ) );
          }
          else
          {
               printf( "\n" );
          }
          exit( EXIT_FAILURE );
     }

     /* Ask for the password. */

     printf( "Password: " );
     fflush( stdout );

     ch = pos = 0;
     while( ch != '\n' )
     {
          ch = ( char )fgetc( stdin );
          if ( !feof( stdin ) && !ferror( stdin ) )
          {
               if ( ch != '\n' )
               {
                    if ( ch == 8 || ch == 127 )
                    {
                         if ( pos > 0 )
                         {
                              pos--;
                              pass[ pos ] = 0;
                              printf( "%c%c%c", 8, 32, 8 );
                              fflush( stdout );
                         }
                    }
                    else
                    {
                         pass[ pos ] = ch;
                         pos++;
                         printf( "*" );
                         fflush( stdout );
                    }
               }    /* if ( ch != '\n' ) */
          }    /* if ( !feof( stdin ) && !ferror( stdin ) ) */
          else
          {
               ch = '\n';  /* Stop the loop. */
               if ( ferror( stdin ) )
               {
                    clearerr( stdin );
               }
          }
     }    /* while( ch != '\n' ) */

     /* Turn ECHO and ICANON back on. */

     data.c_lflag = ISIG | ICANON | ECHO | ECHOE | ECHOK | IEXTEN;
     ret = tcsetattr( 0, TCSANOW, &data );
     if ( ret != 0 )
     {
          ret = errno;
          printf( "\nSomething went wrong. (4)" );
          if ( ret != 0 )
          {
               printf( "  Error: %s.\n", strerror( ret ) );
          }
          else
          {
               printf( "\n" );
          }
          exit( EXIT_FAILURE );
     }

     /* Show the results. */

     printf( "\n\nuser: \"%s\"\n", user );
     printf( "pass: \"%s\"\n\n", pass );
     exit( EXIT_SUCCESS);
}

int read_stdin( char *buffer, const int length, const char *prompt )
{
     int exit_loop, size, use_prompt;

     /* We can't store anything without a buffer to put it in. */

     if ( buffer == NULL )
     {
          errno = EFAULT;
          return 1;
     }

     /* See if the buffer isn't long enough to be useful or is too long. */

     if ( length < 3 || length > 1000000 )
     {
          errno = EINVAL;
          return 1;
     }

     /* Find out if we have received a useful prompt to use. */

     if ( prompt == NULL )
     {
          use_prompt = 0;
     }
     else if ( prompt[ 0 ] == 0 )
     {
          use_prompt = 0;
     }
     else
     {
          use_prompt = 1;
     }

     exit_loop = 0;
     do
     {
          if ( use_prompt == 1 )
          {
               printf( "%s", prompt );
               fflush( stdout );
          }
          else if ( use_prompt == ( -1 ) ) /* This might happen later. */
          {
               use_prompt = 1;
          }

          if ( fgets( buffer, length, stdin ) == NULL )
          {

               /* If we've reached the end of the file on stdin then one */
               /* two things has happened.  Either the user entered the  */
               /* end of file character as their input or the user has   */
               /* redirected stdin on the command line so our program    */
               /* will read from a file instead of the user's keyboard.  */

               if ( feof( stdin ) )
               {
                    errno = ENOMSG;
               }

               /* If we don't have an end of file condition */
               /* then fgets() has already set the errno.   */

               return 1;
          }

          /* See if anything else went wrong. */

          if ( ferror( stdin ) )
          {
               return 1;
          }

          size = strlen( buffer );
          if ( size > 1 )
          {
               size--;
               if ( buffer[ size ] == '\n' )
               {
                    buffer[ size ] = 0;  /* Remove the newline. */
               }
               exit_loop = 1;  /* We're done. */
          }
          else if ( use_prompt == 1 && size == 1 && buffer[ 0 ] == '\n' )
          {
               use_prompt = ( -1 );  /* Silently loop back around. */
          }

     }    while( exit_loop == 0 );
     return 0;
}

/* EOF passwd_stars.c */