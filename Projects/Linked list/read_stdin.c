/*

     read_stdin.c

     This function uses fgets() to read input from stdin.  If a
     character string is provided as a prompt it will be printed
     first.  Input only containing a newline will be ignored.
     The newline character on the end of the line will be removed.

     Parameters:

     char *buffer holds the input string.

     const int length tells this function how long the buffer is in bytes.

     const char *prompt tells this function to print the prompt string
                        before requesting input.  Use NULL to disable
                        this option.

     Returns 0 on success and 1 on error.
   
     Written by Matthew Campbell.

     This file may be freely copied and freely distributed.

*/

#ifndef _READ_STDIN_C
#define _READ_STDIN_C

#include <errno.h>
#include <stdio.h>
#include <string.h>

/* It seems some compiler packages have incomplete include files... */

#ifndef EFAULT
#define EFAULT 14
#endif

#ifndef EINVAL
#define EINVAL 22
#endif

#ifndef ENOMSG
#define ENOMSG 42
#endif

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

#endif  /* _READ_STDIN_C */

/* EOF read_stdin.c */
