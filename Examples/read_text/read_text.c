/*

     read_text.c

     This function replaces fgets(3) to read input from a text stream.
     If a character string is provided as a prompt it will be printed
     first.  Input only containing a newline will be ignored.
     The newline character on the end of the line will be removed.

     Parameters:

     char *buffer holds the input string.

     const int length tells this function how long the buffer is in bytes.

     FILE *fp tells the function what stream to read from.

     const char *prompt tells this function to print the prompt string
                        before requesting input.  Use NULL to disable
                        this option.

     const int prompt_once will only print the prompt once.

     Returns the number of bytes read, or -1 on error.
   
     Written by Matthew Campbell on Tuesday May 21, 2019.

     This function sets errno in the following ways:

          Zero (0): Everything worked just fine.

          EBADF: fp is a NULL pointer.

          EFAULT: buffer is a NULL pointer.

          EINVAL: length or prompt_once holds an unacceptable value.

          ENODATA: fp has an end of file condition.

          EBADFD: fp has an error condition.

          EILSEQ: A NULL byte was encountered in the text stream.

          ENOBUFS: buffer filled up before a newline character was read.

     errno may still be set even if a positive integer value is returned.

*/

#ifndef _READ_TEXT_C
#define _READ_TEXT_C

#include <errno.h>
#include <stdio.h>
#include <string.h>

/* It seems some compiler packages have incomplete header files... */

#ifndef EBADF
#define EBADF 9
#endif

#ifndef EFAULT
#define EFAULT 14
#endif

#ifndef EINVAL
#define EINVAL 22
#endif

#ifndef ENODATA
#define ENODATA 61
#endif

#ifndef EBADFD
#define EBADFD 77
#endif

#ifndef EILSEQ
#define EILSEQ 84
#endif

#ifndef ENOBUFS
#define ENOBUFS 105
#endif

int read_text( char *buffer, const int length, FILE *fp,
               const char *prompt, const int prompt_once )
{
     int byte, exit_loop, pos, size, use_prompt;

     /* We can't store anything without a buffer to put it in. */

     if ( buffer == NULL )
     {
          errno = EFAULT;
          return ( -1 );
     }

     /* See if the buffer isn't long enough to be useful or is too long. */

     if ( length < 2 || length > 1000000 )
     {
          errno = EINVAL;
          return ( -1 );
     }

     /* Make sure we have a valid file pointer. */

     if ( fp == NULL )
     {
          errno = EBADF;
          return ( -1 );
     }
     if ( ferror( fp ) )
     {
          errno = EBADFD;
          return ( -1 );
     }
     if ( feof( fp ) )
     {
          errno = ENODATA;
          return ( -1 );
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

     if ( prompt_once != 0 && prompt_once != 1 )
     {
          errno = EINVAL;
          return ( -1 );
     }

     exit_loop = 0;
     do
     {
          if ( use_prompt == 1 )
          {
               printf( "%s", prompt );
               fflush( stdout );
               if ( prompt_once == 1 )
               {
                    use_prompt = 0;
               }
          }

          size = length - 1;  /* Leave room for the NULL byte. */
          for( pos = 0; exit_loop == 0 && pos < size; pos++ )
          {
               byte = fgetc( fp );
               if ( byte == EOF )
               {
                    buffer[ pos ] = 0;
                    errno = ENODATA;
                    return pos;
               }
               if ( ferror( fp ) )
               {
                    buffer[ pos ] = 0;
                    errno = EBADFD;
                    return ( -1 );
               }
               if ( byte == 0 )  /* We read a NULL byte. */
               {
                    buffer[ pos ] = 0;
                    errno = EILSEQ;
                    return ( -1 );
               }
               if ( byte != '\n' )
               {
                    buffer[ pos ] = ( char )byte;
               }
               else  /* byte == '\n' */
               {
                    if ( pos == 0 )
                    {
                         pos = size;
                    }
                    else
                    {
                         buffer[ pos ] = 0;
                         pos--;
                         exit_loop = 1;
                    }
               }    /* if ( byte != 10 ) */
          }    /* for( pos = 0; exit_loop == 0 && pos < size; pos++ ) */
     }    while( exit_loop == 0 || pos == ( size + 1 ) );

     if ( pos == size )  /* We ran out of room. */
     {
          buffer[ pos ] = 0;
          errno = ENOBUFS;
          return pos;
     }
     else
     {
          if ( feof( fp ) )
          {
               errno = ENODATA;
          }
          else
          {
               errno = 0;
          }
          return pos;  /* Successful exit. */
     }
}

#endif  /* _READ_TEXT_C */

/* EOF read_text.c */
