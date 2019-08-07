/*

     mksc.c
     Make Script.
     Written by Matthew Campbell.  (Wednesday June 20, 2018)

*/

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define USAGE "\
Usage: mksc <files per directory> <output script> <target directory>"

int main( int argc, char **argv )
{
     char buffer[ 1025 ];
     int  count, dir, files, loop, size;
     FILE *fp;

     if ( argc != 4 )
     {
          fprintf( stderr, "%s\n", USAGE );
          exit( EXIT_FAILURE );
     }
     if ( sscanf( argv[ 1 ], "%d", &files ) != 1 )
     {
          fprintf( stderr, "%s\n", USAGE );
          exit( EXIT_FAILURE );
     }
     if ( files < 1 || files > 1000000 )
     {
          fprintf( stderr, "Invalid number of files per directory.\n" );
          exit( EXIT_FAILURE );
     }
     fp = fopen( argv[ 2 ], "w" );
     if ( fp == NULL )
     {
          fprintf( stderr, "Failed to open the output file." );
          fprintf( stderr, "Error: %s.\n", strerror( errno ) );
          exit( EXIT_FAILURE );
     }
     fprintf( fp, "#\n# A copying script.\n#\n" );
     dir = 1;
     count = 0;
     fprintf( fp, "mkdir \"%s/%d\"\n", argv[ 3 ], dir );
     while( !feof( stdin ) )
     {
          do
          {
               loop = 0;
               if ( fgets( buffer, 1024, stdin ) == NULL &&
                    !feof( stdin ) )
               {
                    fprintf( stderr,
                             "There was a problem with fgets().\n" );
                    fprintf( stderr, "Error: %s.\n", strerror( errno ) );
                    exit( EXIT_FAILURE );
               }
               if ( feof( stdin ) )
               {
                    fprintf( fp, "#\n# EOF\n" );
                    fclose( fp );
                    fp = NULL;
                    exit( EXIT_SUCCESS );
               }
               if ( buffer[ 0 ] == 0 )
               {
                    fprintf( stderr, "No data returned by fgets().\n" );
                    exit( EXIT_FAILURE );
               }
               size = strlen( buffer );
               if ( size == 1 && buffer[ 0 ] == '\n' )
               {
                    loop = 1;
               }
               else
               {
                    size--;
                    buffer[ size ] = 0;
               }
          }    while( loop == 1 );
          fprintf( fp, "cp \"%s\" \"%s/%d/%s\"\n", buffer, argv[ 3 ], dir,
                   buffer );
          count++;
          if ( count == files )
          {
               count = 0;
               dir++;
               fprintf( fp, "mkdir \"%s/%d\"\n", argv[ 3 ], dir );
          }
     }
     return 0;  /* Not like we'll get to this point anyway. */
}

/* EOF */