/*

     wc.c

     Word count.

     This program is intended to serve as a simple example rather
     than including every function included in the word count program
     documented in the Linux man page.  It would be easy enough to
     add the extra functioning so it could do so, but I didn't believe
     it would be worth the work to add functioning that I would never
     be likely to use.
    
     Written by Matthew Campbell on Sunday July 1, 2018.

*/

#include <stdio.h>
#include <string.h>

/* Define USE_WORDS if you want an explanation with the numerical output. */

#define USE_WORDS

int main( int argc, char **argv )
{
     FILE *fp;
     char ch;
     int bytes, lines, use_stdin, words;
     int last_newline, last_non_space, last_space;

     fp = NULL;
     bytes = lines = words = 0;
     last_newline = last_non_space = last_space = -1;

     if ( argc != 2 )
     {
          fprintf( stderr, "Usage: wc <filename> or - for stdin.\n" );
          return 1;
     }

     if ( !strcmp( argv[ 1 ], "-" ) )
     {
          use_stdin = 1;
          fp = stdin;
     }
     else
     {
          use_stdin = 0;
          fp = fopen( argv[ 1 ], "r" );
          if ( fp == NULL )
          {
               fprintf( stderr, "Failed to open file: \"%s\"\n",
                        argv[ 1 ] );
               return 1;
          }
     }

     /* Keep reading until we reach the end  */
     /* of the file or something goes wrong. */

     do
     {
          ch = ( char )fgetc( fp );
          if ( ferror( fp ) )
          {
               fprintf( stderr, "Something went wrong while reading.\n" );
               clearerr( fp );
               fclose( fp );
               fp = NULL;
               return 1;
          }
          if ( !feof( fp ) )
          {
               bytes++;
               if ( ch == 32 )  /* Space */
               {
                    last_space = bytes;
                    if ( last_non_space != ( -1 ) )
                    {
                         words++;
                    }
               }
               else if ( ch == '\n' )  /* Newline */
               {
                    last_newline = bytes;
                    lines++;
                    if ( last_non_space != ( -1 ) )
                    {
                         words++;
                    }
               }
               else  /* Non-space */
               {
                    last_non_space = bytes;
               }
          }
     }    while( !feof( fp ) );

     /* Now we need to handle some special cases. */

     /* If we had characters, but didn't have any spaces, */
     /* and the input didn't end with a newline...        */

     if ( last_non_space != ( -1 ) && last_newline != bytes )
     {
          words++;
     }

     /* If the input didn't end with a newline... */

     if ( last_newline != bytes && bytes != 0 )
     {
          lines++;
     }

     fclose( fp );
     fp = NULL;

#ifdef USE_WORDS

     if ( lines == 1 )
     {
          printf( "1 line\n" );
     }
     else
     {
          printf( "%d lines\n", lines );
     }
     if ( words == 1 )
     {
          printf( "1 word\n" );
     }
     else
     {
          printf( "%d words\n", words );
     }
     if ( bytes == 1 )
     {
          printf( "1 byte\n" );
     }
     else
     {
          printf( "%d bytes\n", bytes );
     }

#else

     printf( "%d\n%d\n%d\n", lines, words, bytes );

#endif

     return 0;
}

/* EOF wc.c */