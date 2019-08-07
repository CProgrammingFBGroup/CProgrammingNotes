/*

     wrap.c

     Wraps long lines in text files.

     If the third option is not specified then stdout will be used.
     If the second option is not specified then stdin will be used.

     Written by Matthew Campbell on Monday June 11, 2018.

*/

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define USAGE "<max line length> [input file] [output file]\n"

int main( int argc, char **argv )
{
     char buffer[ 201 ], ch;
     int  last, last_alt, last_space, length, pos, stop;
     FILE *infile, *outfile;

     if ( argc < 2 || argc > 4 )
     {
          fprintf( stderr, "Usage: %s %s", argv[ 0 ], USAGE );
          exit( EXIT_FAILURE );
     }
     if ( argc == 2 )
     {
          if ( !strcmp( argv[ 1 ], "-h" ) )
          {
               fprintf( stderr, "Usage: %s %s", argv[ 0 ], USAGE );
               exit( EXIT_SUCCESS );
          }
          if ( sscanf( argv[ 1 ], "%d", &length ) != 1 )
          {
               fprintf( stderr, "Usage: %s %s", argv[ 0 ], USAGE );
               exit( EXIT_FAILURE );
          }
          if ( length < 5 )
          {
               fprintf( stderr, "\
%s: You must use at least 5 columns per line.\n", argv[ 0 ] );
               exit( EXIT_FAILURE );
          }
          if ( length > 200 )
          {
               fprintf( stderr, "\
%s: You must not use more than 200 columns per line.\n",
                        argv[ 0 ] );
               exit( EXIT_FAILURE );
          }
          infile = stdin;
          outfile = stdout;
     }
     else if ( argc == 3 )
     {
          if ( sscanf( argv[ 1 ], "%d", &length ) != 1 )
          {
               fprintf( stderr,
                        "Usage: %s %s", argv[ 0 ], USAGE );
               exit( EXIT_FAILURE );
          }
          if ( length < 5 )
          {
               fprintf( stderr, "\
%s: You must use at least 5 columns per line.\n", argv[ 0 ] );
               exit( EXIT_FAILURE );
          }
          if ( length > 200 )
          {
               fprintf( stderr, "\
%s: You must not use more than 200 columns per line.\n",
                        argv[ 0 ] );
               exit( EXIT_FAILURE );
          }
          infile = fopen( argv[ 2 ], "r" );
          if ( infile == NULL )
          {
               fprintf( stderr, "\
%s: Failed to open input file \"%s\" for reading.\n",
                        argv[ 0 ], argv[ 2 ] );
               exit( EXIT_FAILURE );
          }
          outfile = stdout;
     }
     else  /* argc == 4 */
     {
          if ( sscanf( argv[ 1 ], "%d", &length ) != 1 )
          {
               fprintf( stderr,
                        "Usage: %s <number of columns per line> \
[input file] [output file]\n", argv[ 0 ] );
               exit( EXIT_FAILURE );
          }
          if ( length < 5 )
          {
               fprintf( stderr, "\
%s: You must use at least 5 columns per line.\n", argv[ 0 ] );
               exit( EXIT_FAILURE );
          }
          if ( length > 200 )
          {
               fprintf( stderr, "\
%s: You must not use more than 200 columns per line.\n",
                        argv[ 0 ] );
               exit( EXIT_FAILURE );
          }
          infile = fopen( argv[ 2 ], "r" );
          if ( infile == NULL )
          {
               fprintf( stderr, "\
%s: Failed to open input file \"%s\" for reading.\n",
                        argv[ 0 ], argv[ 2 ] );
               exit( EXIT_FAILURE );
          }
          outfile = fopen( argv[ 3 ], "w" );
          if ( outfile == NULL )
          {
               fprintf( stderr, "\
%s: Failed to open output file \"%s\" for writing.\n",
                        argv[ 0 ], argv[ 3 ] );
               fclose( infile );
               exit( EXIT_FAILURE );
          }
     }
     for( pos = 0; pos < 201; pos++ )
     {
          buffer[ pos ] = 0;
     }
     pos = stop = 0;
     last_alt = last_space = ( -1 );
     length--;
     do
     {
          ch = ( char )fgetc( infile );
          if ( feof( infile ) )
          {
               stop = 1;
               buffer[ pos ] = 0;
               if ( pos != 0 )
               {

     /* Don't put a newline on the end if the input file didn't. */

                    fprintf( outfile, "%s", buffer );
                    if ( ferror( outfile ) )
                    {
                         fprintf( stderr, "\
%s: Error while writing to the output file.\n", argv[ 0 ] );
                         fprintf( stderr, "\
%s: Error message: %s.\n", argv[ 0 ], strerror( errno ) );
                         clearerr( outfile );
                    }
                    else
                    {
                         fflush( outfile );
                         if ( ferror( outfile ) )
                         {
                              fprintf( stderr, "\
%s: Error while writing to the output file.\n", argv[ 0 ] );
                              fprintf( stderr, "\
%s: Error message: %s.\n", argv[ 0 ], strerror( errno ) );
                              clearerr( outfile );
                         }
                    }
               }    /* if ( pos != 0 ) */
          }
          else if ( ferror( infile ) )
          {
               stop = 1;
               fprintf( stderr, "\
%s: An error occured while reading from the input file.\n",
                        argv[ 0 ] );
               fprintf( stderr, "\
%s: Error message: %s.\n", argv[ 0 ], strerror( errno ) );
          }
          else
          {

     /* Store the character in the buffer. */

               buffer[ pos ] = ch;

     /* If this is a newline, write out the buffer. */

               if ( ch == '\n' )
               {
                    buffer[ ( pos + 1 ) ] = 0;
                    fprintf( outfile, "%s", buffer );
                    if ( ferror( outfile ) )
                    {
                         stop = 1;
                         fprintf( stderr, "\
%s: Error while writing to the output file.\n", argv[ 0 ] );
                         fprintf( stderr, "\
%s: Error message: %s.\n", argv[ 0 ], strerror( errno ) );
                         clearerr( outfile );
                    }
                    pos = ( -1 );
                    last_alt = last_space = ( -1 );
               }

     /* If this character is a space, keep track of it. */

               else if ( ch == 32 )
               {
                    last_space = pos;


               }

     /* If this is an alternate breaking point, keep track. */

               else if ( ch == '!'  || ch == '&' ||
                         ch == '*'  || ch == ',' ||
                         ch == '-'  || ch == '.' ||
                         ch == '/'  || ch == ':' ||
                         ch == ';'  || ch == '=' ||
                         ch == '?'  || ch == '@' ||
                         ch == '\\' || ch == '_' )
               {
                    last_alt = pos;
               }

     /* Have we reached our maximum line length? */

               if ( length == pos )
               {

     /* Did we encounter any spaces? */

                    if ( last_space != ( -1 ) )
                    {
                         buffer[ last_space ] = 0;
                         fprintf( outfile, "%s\n", buffer );
                         if ( ferror( outfile ) )
                         {
                              stop = 1;
                              fprintf( stderr, "\
%s: Error while writing to the output file.\n", argv[ 0 ] );
                              fprintf( stderr, "\
%s: Error message: %s.\n", argv[ 0 ], strerror( errno ) );
                              clearerr( outfile );
                         }
                         else if ( pos > last_space )
                         {

/* Moving this block of code back a little to get some room. */

/* Move over everything in the buffer that wasn't written yet. */

          last = last_space + 1;
          for( pos = 0; last <= length; last++, pos++ )
          {
               buffer[ pos ] = buffer[ last ];
          }
          buffer[ pos ] = 0;
          if ( last_alt != ( -1 ) )
          {
               last_alt = last_alt - ( last_space + 1 );

     /* If it was already written then reset the reference. */

               if ( last_alt < 0 )
               {
                    last_alt = ( -1 );
               }
          }
          last_space = ( -1 );
          pos--;

/* Restoring normal indentation here. */

                         }

     /* Nothing else in the buffer to write out. */

                         else  /* pos == last_space */
                         {

     /* pos will be incremented to zero soon. */

                              pos = ( -1 );
                              last_alt = last_space = ( -1 );
                         }
                    }

     /* Did we encounter an alternate breaking point instead? */

                    else if ( last_alt != ( -1 ) )
                    {
                         last_alt++;
                         ch = buffer[ last_alt ];
                         buffer[ last_alt ] = 0;
                         fprintf( outfile, "%s\n", buffer );
                         buffer[ last_alt ] = ch;
                         last = last_alt;
                         last_alt--;
                         if ( ferror( outfile ) )
                         {
                              stop = 1;
                              fprintf( stderr, "\
%s: Error while writing to the output file.\n", argv[ 0 ] );
                              fprintf( stderr, "\
%s: Error message: %s.\n", argv[ 0 ], strerror( errno ) );
                              clearerr( outfile );
                         }

/* Moving this block of code back a little to get some room. */

/* Move over everything in the buffer that wasn't written yet. */

          for( pos = 0; last <= length; last++, pos++ )
          {
               buffer[ pos ] = buffer[ last ];
          }
          buffer[ pos ] = 0;
          last_alt = ( -1 );
          pos--;

/* Restoring normal indentation here. */

                    }
                    else  /* Nothing to use. */
                    {

     /* Be safe.  We don't want an error to cause trouble. */

                         buffer[ ( length + 1 ) ] = 0;

     /* Write out what we've got. */

                         fprintf( outfile, "%s\n", buffer );
                         pos = ( -1 );
                         if ( ferror( outfile ) )
                         {
                              stop = 1;
                              fprintf( stderr, "\
%s: Error while writing to the output file.\n", argv[ 0 ] );
                              fprintf( stderr, "\
%s: Error message: %s.\n", argv[ 0 ], strerror( errno ) );
                              clearerr( outfile );
                         }
                    }
               }    /* if ( length == pos ) */
          }    /* !feof( infile ) && !ferror( infile ) */
          pos++;
     }    while( !stop );
     fclose( infile );
     fclose( outfile );
     return 0;
}

/* EOF wrap.c */