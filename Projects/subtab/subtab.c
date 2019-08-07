/*

     subtab.c

     Substitutes all occurances of the ~ character with a tab character.

     Written by Matthew Campbell on Tuesday
     March 12, 2019 and Wednesday March 13, 2019.

*/

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

/*

     USE_CHAR is the character that will
     be swapped with the tab character.

*/

#define USE_CHAR '~'
#define TAB 9

/* TEMP_FILE is the name of the temp file to use. */

#define TEMP_FILE "tempfile"

int main( int argc, char **argv )
{
     char ch;
     int exit_loop, flag, pos, ret, save_errno;
     struct stat ptr;
     FILE *fp, *temp;

     /* Make sure we don't already have a file named tempfile. */

     errno = 0;
     ret = stat( TEMP_FILE, &ptr );
     if ( ret == 0 )
     {
          fprintf( stderr, "\
There is already a file named \"%s\" in the current directory.\n",
                   TEMP_FILE );
          exit( EXIT_FAILURE );
     }
     if ( ret != 0 && errno != ENOENT )
     {
          save_errno = errno;
          fprintf( stderr, "\
There may already be a file named \"%s\" in the current\n\
directory, but something went wrong when trying to stat it.\n",
                   TEMP_FILE );
          if ( save_errno != 0 )
          {
               fprintf( stderr, "Error: %s.\n", strerror( save_errno ) );
          }
          fprintf( stderr, "\n" );
          exit( EXIT_FAILURE );
     }

     /* Make sure we have one or more command line arguments. */

     if ( argc == 1 )
     {
          fprintf( stderr, "Usage: %s <filename> [filename...]\n",
                   argv[ 0 ] );
          fprintf( stderr, "\
Substitutes all occurances of the %c character with the tab character.\n",
                   USE_CHAR );
          exit( EXIT_FAILURE );
     }

     /* Process each file named on the command line. */

     flag = 0;  /* Error flag */
     for( pos = 1; pos < argc && flag == 0; pos++ )
     {
          errno = 0;
          fp = fopen( argv[ pos ], "r" );
          if ( fp == NULL )
          {
               save_errno = errno;
               fprintf( stderr, "\
Failed to open the file \"%s\" for processing.\n", argv[ pos ] );
               if ( save_errno != 0 )
               {
                    fprintf( stderr, "Error: %s.\n",
                             strerror( save_errno ) );
               }
               fprintf( stderr, "\n" );
               flag = 1;
          }
          else  /* The file to be processed has been opened. */
          {
               /* Open the temp file. */

               errno = 0;
               temp = fopen( TEMP_FILE, "w" );
               if ( temp == NULL )
               {
                    save_errno = errno;
                    fprintf( stderr, "\
Failed to open the temp file \"%s\".\n", TEMP_FILE );
                    if ( save_errno != 0 )
                    {
                         fprintf( stderr, "Error: %s.\n",
                                  strerror( save_errno ) );
                    }
                    fprintf( stderr, "\n" );

                    /* Close the file that was to be processed. */

                    errno = 0;
                    ret = fclose( fp );
                    if ( ret != 0 )
                    {
                         save_errno = errno;
                         fprintf( stderr, "\
Failed to close the file \"%s\"\n", argv[ pos ] );
                         if ( save_errno != 0 )
                         {
                              fprintf( stderr, "Error: %s.\n",
                                       strerror( save_errno ) );
                         }
                         fprintf( stderr, "\n" );
                         abort();  /* Crash and burn. */
                    }    /* if ( ret != 0 ) */
                    exit( EXIT_FAILURE );
               }    /* if ( temp == NULL ) */

               /* Process the current file. */

               fprintf( stdout, "Processing \"%s\"\n", argv[ pos ] );

               exit_loop = 0;
               while( exit_loop == 0 && !feof( fp ) && !ferror( fp ) &&
                      !ferror( temp ) )
               {
                    errno = 0;
                    ch = ( char )fgetc( fp );
                    save_errno = errno;
                    if ( ferror( fp ) )
                    {
                         fprintf( stderr, "\
Something went wrong while reading from the file \"%s\"\n", argv[ pos ] );
                         if ( save_errno != 0 )
                         {
                              fprintf( stderr, "Error: %s.\n",
                                       strerror( save_errno ) );
                         }
                         fprintf( stderr, "\n" );
                         exit_loop = 1;
                         flag = 1;
                    }

                    /* Write the character to the temp file. */

                    if ( exit_loop == 0 && !feof( fp ) &&
                         !ferror( fp ) && !ferror( temp ) )
                    {
                         errno = 0;
                         if ( ch == USE_CHAR )
                         {
                              fputc( TAB, temp );
                         }
                         else
                         {
                              fputc( ch, temp );
                         }
                         if ( ferror( temp ) )
                         {
                              fprintf( stderr, "\
Something went wrong while writing to the temp file \"%s\".\n",
                                       TEMP_FILE );
                              if ( save_errno != 0 )
                              {
                                   fprintf( stderr, "Error: %s.\n",
                                            strerror( save_errno ) );
                              }
                              fprintf( stderr, "\n" );

                              exit_loop = 1;
                              flag = 1;
                         }    /* if ( ferror( temp ) ) */
                    }    /* if ( exit_loop == 0 && !feof( fp ) &&
                                 !ferror( fp ) && !ferror( temp ) ) */

               }    /* while( exit_loop == 0 && !feof( fp ) &&
                              !ferror( fp ) && !ferror( temp ) ) */

               /* Close the temp file. */

               if ( !ferror( temp ) )
               {
                    errno = 0;
                    fflush( temp );
                    save_errno = errno;
                    if ( ferror( temp ) )
                    {
                         fprintf( stderr, "\
Something went wrong when writing to the temp file \"%s\".\n",
                                  TEMP_FILE );
                         if ( save_errno != 0 )
                         {
                              fprintf( stderr, "Error: %s.\n",
                                       strerror( save_errno ) );
                         }
                         fprintf( stderr, "\n" );
                         clearerr( temp );
                         flag = 1;
                    }    /* if ( ferror( temp ) ) */
               }    /* if ( !ferror( temp ) ) */
               else
               {
                    clearerr( temp );
                    flag = 1;
               }

               errno = 0;
               ret = fclose( temp );
               if ( ret != 0 )
               {
                    save_errno = errno;
                    fprintf( stderr, "\
Failed to close the temp file.\n" );
                    if ( save_errno != 0 )
                    {
                         fprintf( stderr, "Error: %s.\n",
                                  strerror( save_errno ) );
                    }
                    fprintf( stderr, "\n" );
                    abort();  /* Crash and burn. */
               }
               temp = NULL;

               /* Close the file that was to be processed. */

               if ( ferror( fp ) )
               {
                    clearerr( fp );
                    flag = 1;
               }
               errno = 0;
               ret = fclose( fp );
               if ( ret != 0 )
               {
                    save_errno = errno;
                    fprintf( stderr, "\
Failed to close the file \"%s\"\n", argv[ pos ] );
                    if ( save_errno != 0 )
                    {
                         fprintf( stderr, "Error: %s.\n",
                                  strerror( save_errno ) );
                    }
                    fprintf( stderr, "\n" );
                    abort();  /* Crash and burn. */
               }
               fp = NULL;
          }    /* if ( fp == NULL ) */

          /* Replace the original file with the temp file. */

          if ( flag == 0 )
          {
               errno = 0;
               ret = unlink( argv[ pos ] );
               if ( ret != 0 )
               {
                    save_errno = errno;
                    fprintf( stderr, "\
Failed to remove the file \"%s\" so it could be replaced.\n",
                             argv[ pos ] );
                    if ( save_errno != 0 )
                    {
                         fprintf( stderr, "Error: %s.\n",
                                  strerror( save_errno ) );
                    }
                    fprintf( stderr, "\n" );

                    /* Remove the temp file. */

                    errno = 0;
                    ret = unlink( TEMP_FILE );
                    if ( ret != 0 )
                    {
                         save_errno = errno;
                         fprintf( stderr, "\
Failed to remove the temp file \"%s\".\n", TEMP_FILE );
                         if ( save_errno != 0 )
                         {
                              fprintf( stderr, "Error: %s.\n",
                                       strerror( save_errno ) );
                         }
                         fprintf( stderr, "\n" );
                         abort();  /* Crash and burn. */
                    }
                    flag = 1;
               }
               else  /* ret == 0 */
               {
                    errno = 0;
                    ret = rename( TEMP_FILE, argv[ pos ] );
                    if ( ret != 0 )
                    {
                         save_errno = errno;
                         fprintf( stderr, "\
Failed to rename the temp file \"%s\" to \"%s\".\n", TEMP_FILE,
                                  argv[ pos ] );
                         if ( save_errno != 0 )
                         {
                              fprintf( stderr, "Error: %s.\n",
                                       strerror( save_errno ) );
                         }
                         fprintf( stderr, "\n" );
                         abort();  /* Crash and burn. */
                    }
               }    /* if ( ret != 0 ) */
          }
          else  /* flag == 1 */
          {
               /* Remove the temp file. */

               errno = 0;
               ret = unlink( TEMP_FILE );
               if ( ret != 0 )
               {
                    save_errno = errno;
                    fprintf( stderr, "\
Failed to remove the temp file \"%s\".\n", TEMP_FILE );
                    if ( save_errno != 0 )
                    {
                         fprintf( stderr, "Error: %s.\n",
                                  strerror( save_errno ) );
                    }
                    fprintf( stderr, "\n" );
                    abort();  /* Crash and burn. */
               }
               flag = 1;
          }    /* if ( flag == 0 ) */
     }    /* for( pos = 1; pos < argc; pos++ ) */

     /* Exit. */

     if ( flag == 0 )
     {
          fprintf( stdout, "Successful exit.\n" );
          exit( EXIT_SUCCESS );
     }
     else
     {
          fprintf( stdout, "\
The program encountered one or more problems.\n" );
          exit( EXIT_FAILURE );
     }
}

/* EOF subtab.c */
