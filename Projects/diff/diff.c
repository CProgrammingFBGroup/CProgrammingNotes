/*

     diff.c
     Compares binary files and looks for differences.
     Written by Matthew Campbell on Monday July 9, 2018.

*/

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main( int argc, char **argv )
{
     FILE *f1, *f2;
     char ch1, ch2;
     int loop, offset, same;

     if ( argc != 3 )
     {
          fprintf( stderr, "Usage: diff <file1> <file2>\n" );
          return 1;
     }
     if ( ( f1 = fopen( argv[ 1 ], "r" ) ) == NULL )
     {
          fprintf( stderr, "Failed to open the first file.\n" );
          fprintf( stderr, "Error: %s\n", strerror( errno ) );
          return 1;
     }
     if ( ( f2 = fopen( argv[ 2 ], "r" ) ) == NULL )
     {
          fprintf( stderr, "Failed to open the second file.\n" );
          fprintf( stderr, "Error: %s\n", strerror( errno ) );
          fclose( f1 );
          return 1;
     }
     loop = 1;
     offset = 0;
     same = 0;
     do
     {
          ch1 = fgetc( f1 );
          ch2 = fgetc( f2 );
          offset++;
          if ( feof( f1 ) && !feof( f2 ) )
          {
               fprintf( stdout, "\
The second file is longer than the first file, but matches the first \
file up to the length of the first file.\n" );
               loop = 0;
          }
          else if ( feof( f2 ) && !feof( f1 ) )
          {
               fprintf( stdout, "\
The first file is longer than the second file, but matches the second \
file up to the length of the second file.\n" );
               loop = 0;
          }
          else if ( ferror( f1 ) )
          {
               fprintf( stderr,
                        "Something went wrong with the first file.\n" );
               fprintf( stderr, "Error: %s\n", strerror( errno ) );
               clearerr( f1 );
               fprintf( stdout, "Offset: %d\n", offset );
               loop = 0;
          }
          else if ( ferror( f2 ) )
          {
               fprintf( stderr,
                        "Something went wrong with the second file.\n" );
               fprintf( stderr, "Error: %s\n", strerror( errno ) );
               clearerr( f2 );
               fprintf( stdout, "Offset: %d\n", offset );
               loop = 0;
          }
          else if ( feof( f1 ) && feof( f2 ) )
          {
               loop = 0;  /* We're done. */
               same = 1;
          }
          else if ( ch1 != ch2 )
          {
               fprintf( stdout, "\
The first difference between the two files was found at byte offset: %d.\n",
                        offset );
               loop = 0;
          }
     }    while( loop == 1 );
     if ( same == 1 )
     {
          fprintf( stdout, "Both files are the same.\n" );
     }
     fclose( f1 );
     fclose( f2 );
     return 0;
}

/* EOF diff.c */
