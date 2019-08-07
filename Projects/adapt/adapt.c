/*

     adapt.c

     Adapts scripts designed to run on Linux or Unix systems
     so they can run on an Android device under Termux.

     Written by Matthew Campbell on Saturday June 15, 2019.

     Updated by Matthew Campbell on Sunday June 16, 2019
     to use the functions close_files() and remove_temp_file().

*/

/*

     This program will adapt text script files that begin with either

     #!/bin/

     or

     #!/usr/

     to make then compatible with the Termux app by changing the first
     line in the file according to the defined values for BIN_DIR and
     USR_DIR.  If the file to be adapted started with

     #!/bin/bash

     then it would be adapted to

     #!/data/data/com.termux/files/bin/bash

     It may be worth noting that bash is actually in usr/bin/ in Termux.
     This can be fixed by creating a bin/ directory as specified by
     BIN_DIR and then creating a symbolic link with the ln command to
     point to the executable program in usr/bin/.  It would also be
     possible to create a symbolic link for the directory instead so
     that bin/ pointed to usr/bin/ since bin/ does not exist by default
     in Termux.

     Calling abort() causes the program to crash with an exit code of 134.

*/

/*

     This is open source software.  It is provided free of charge and
     without any warranty of any kind whatsoever.  It is provided
     ''AS IS''.  Use this software at your own risk.  The author of
     this software assumes no risk or liability as a result of the
     user's use, misuse, or abuse of this software.  The user assumes
     all risk and liability arising out of the user's use, misuse, or
     abuse of this software.  If you do not like this software, whether
     whole or in part, then you may modify your copy or copies of this
     software for your own use, or choose not to use this software.
     Unmodified copies of this software may be freely distributed.
     This software may be freely copied.  You may keep an unlimited
     number of copies of this software on an unlimited number of
     devices using an unlimited number of methods of your choice
     provided that doing so does not impose any kind of cost, harm,
     or expense upon the author of this software.  Any copies of this
     software shall be kept at the user's expense.  It is recommended
     that the user of this software create backup copies of any files
     they intend to use this software to modify in case something
     goes wrong.  The user bears full responsibility for their own
     carelessness and foolishness, if any.

*/

/* snprintf(3) needs this: */

#define _BSD_SOURCE

/* Include the necessary header files: */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

/* Make sure this error code is defined: */

#ifndef ENOENT
#define ENOENT 2
#endif

/* Define the Termux path names to use: */

#define BIN_DIR "/data/data/com.termux/files/bin/"
#define USR_DIR "/data/data/com.termux/files/usr/"

/* Define the usage string: */

#define USAGE "Usage: adapt <file1> [file2] [file3...]\n"

/* Define the name of the temporary file used by this program: */

#define TEMP_FILE "adapt.tmp"

/*

     Define EXIT_IF_NOT_OPENED if you want the program to fail
     if one of the files listed on the command line could not
     be opened for reading.  If it is left undefined then the
     program will try to process the other files listed on the
     command line.  An exit code of 1 will be returned if no
     files were adapted and at least one file could not be
     opened for reading.

*/

#undef EXIT_IF_NOT_OPENED

/* Function prototypes: */

          void close_files( const char *reason, int save_errno,
                            const char *name, FILE **infile,
                            FILE **outfile );

_Noreturn void remove_temp_file( void );

/* Function definitions: */

int main( int argc, char **argv )
{
     FILE *infile, *outfile;
     char buffer[ 8 ];
     static char str[ 1024 ];  /* Don't put it on the local stack. */
     int bin_length, count, exit_loop, len, num_adapted;
     int open_failed, pos, ret, save_errno, usr_length;
     struct stat stat_buffer;

     /* Return an error message if we are not given anything to do. */

     if ( argc < 2 )
     {
          printf( "%s", USAGE );
          exit( EXIT_FAILURE );
     }

     /* Initialize some variables. */

     infile = outfile = NULL;
     num_adapted = open_failed = 0;

     bin_length = strlen( BIN_DIR ) + 2;
     usr_length = strlen( USR_DIR ) + 2;

     len = 1024;  /* len must be set to the length of str. */

     /* Make sure our temporary file is not present. */

     errno = 0;
     ret = stat( TEMP_FILE, &stat_buffer );
     save_errno = errno;
     if ( ret == ( -1 ) )
     {
          if ( save_errno != ENOENT )  /* No such file or directory. */
          {
               printf( "\
adapt: The temporary file \"%s\" has been found in the current directory.\
\n", TEMP_FILE );
               exit( EXIT_FAILURE );
          }
     }
     else  /* stat(2) did not return an error so the file is present. */
     {
          printf( "\
adapt: The temporary file \"%s\" has been found in the current directory.\
\n", TEMP_FILE );
          exit( EXIT_FAILURE );
     }

     /* Process each file. */

     for( count = 1; count < argc; count++ )
     {
          /* Open the input file for reading. */

          errno = 0;
          infile = fopen( argv[ count ], "r" );
          save_errno = errno;
          if ( infile == NULL )
          {
               open_failed++;
               printf( "adapt: Failed to open \"%s\" for reading.\n",
                       argv[ count ] );
               if ( save_errno != 0 )
               {
                    printf( "adapt: Error: %s.\n",
                            strerror( save_errno ) );
               }

#ifdef EXIT_IF_NOT_OPENED

               printf( "adapt: Program failed.  Exiting.\n" );
               exit( EXIT_FAILURE );

#endif

          }
          else  /* The file has been opened for reading. */
          {
               memset( buffer, 0, 8 );  /* Clear the buffer space. */
               for( pos = 0; pos < 7; pos++ )
               {
                    ret = fgetc( infile );
                    if ( !feof( infile ) && !ferror( infile ) &&
                         ret >= 0 )
                    {
                         buffer[ pos ] = ( char )ret;
                    }
                    else
                    {
                         break;  /* Break the loop. */
                    }
               }

               /* See if it needs to be adapted. */

               if ( strcmp( buffer, "#!/bin/" ) == 0 )
               {
                    /* Open the temporary file for writing. */

                    errno = 0;
                    outfile = fopen( TEMP_FILE, "w" );
                    save_errno = errno;
                    if ( outfile == NULL )
                    {
                         snprintf( str, len, "\
adapt: Failed to open the temporary file \"%s\" for writing.\n",
                                 TEMP_FILE );
                         close_files( str, save_errno, argv[ count ],
                                      &infile, NULL );

                         /* Exit. */

                         printf( "adapt: Program failed.  Exiting.\n" );
                         exit( EXIT_FAILURE );

                    }    /* if ( outfile == NULL ) */

                    /* Write the new path name. */

                    errno = 0;
                    ret = fprintf( outfile, "#!%s", BIN_DIR );
                    save_errno = 0;
                    if ( ret < bin_length )
                    {
                         snprintf( str, len, "\
adapt: Failed to write the new path name to the temporary file \"%s\".\n",
                                 TEMP_FILE );
                         close_files( str, save_errno, argv[ count ],
                                      &infile, &outfile );

                         /* Remove the temporary file. */

                         errno = 0;
                         ret = unlink( TEMP_FILE );
                         save_errno = errno;
                         if ( ret != 0 )
                         {
                              printf( "\
adapt: Failed to remove the temporary file \"%s\".\n", TEMP_FILE );
                              if ( save_errno != 0 )
                              {
                                   printf( "adapt: Error: %s.\n",
                                           strerror( save_errno ) );
                              }
                              printf( "\
adapt: Catastrophic failure.  Aborting.\n" );
                              abort();  /* Crash and burn. */
                         }

                         /* Exit. */

                         printf( "adapt: Program failed.  Exiting.\n" );
                         exit( EXIT_FAILURE );

                    }    /* if ( ret < bin_length ) */
               }
               else if ( strcmp( buffer, "#!/usr/" ) == 0 )
               {
                    /* Open the temporary file for writing. */

                    errno = 0;
                    outfile = fopen( TEMP_FILE, "w" );
                    save_errno = errno;
                    if ( outfile == NULL )
                    {
                         snprintf( str, len, "\
adapt: Failed to open the temporary file \"%s\" for writing.\n",
                                 TEMP_FILE );
                         close_files( str, save_errno, argv[ count ],
                                      &infile, NULL );

                         /* Exit. */

                         printf( "adapt: Program failed.  Exiting.\n" );
                         exit( EXIT_FAILURE );

                    }    /* if ( outfile == NULL ) */

                    /* Write the new path name. */

                    errno = 0;
                    ret = fprintf( outfile, "#!%s", USR_DIR );
                    save_errno = 0;
                    if ( ret < usr_length )
                    {
                         snprintf( str, len, "\
adapt: Failed to write the new path name to the temporary file \"%s\".\n",
                                 TEMP_FILE );
                         close_files( str, save_errno, argv[ count ],
                                      &infile, &outfile );

                         /* Remove the temporary file. */

                         errno = 0;
                         ret = unlink( TEMP_FILE );
                         save_errno = errno;
                         if ( ret != 0 )
                         {
                              printf( "\
adapt: Failed to remove the temporary file \"%s\".\n", TEMP_FILE );
                              if ( save_errno != 0 )
                              {
                                   printf( "adapt: Error: %s.\n",
                                           strerror( save_errno ) );
                              }
                              printf( "\
adapt: Catastrophic failure.  Aborting.\n" );
                              abort();  /* Crash and burn. */
                         }

                         /* Exit. */

                         printf( "adapt: Program failed.  Exiting.\n" );
                         exit( EXIT_FAILURE );

                    }    /* if ( ret < usr_length ) */
               }
               else  /* This file does not need to be adapted. */
               {
                    close_files( NULL, 0, argv[ count ], &infile, NULL );

                    /* Move on to the next file in the list, if any. */

                    continue;
               }

               /*

                    At this point the new path name has been
                    successfully written to the temporary file's
                    buffer space and the rest of the input file
                    needs to be copied to the temporary file.

               */

               exit_loop = 0;
               errno = save_errno = 0;
               do
               {
                    if ( !feof( infile ) && !ferror( infile ) &&
                         !ferror( outfile ) )
                    {
                         errno = 0;
                         ret = fgetc( infile );
                         save_errno = errno;
                         if ( !feof( infile ) && !ferror( infile ) &&
                              ret != ( -1 ) )
                         {
                              errno = 0;
                              if ( fputc( ret, outfile ) == ( -1 ) )
                              {
                                   exit_loop = 1;
                                   save_errno = errno;
                              }
                         }
                         else
                         {
                              exit_loop = 1;
                         }
                    }
                    else
                    {
                         exit_loop = 1;
                    }
               }    while( exit_loop == 0 );

               /* See if anything went wrong with the input file. */

               if ( ferror( infile ) )
               {
                    snprintf( str, len, "\
adapt: Failed to finish copying the input file \"%s\".\n",
                              argv[ count ] );
                    close_files( str, save_errno, argv[ count ],
                                 &infile, &outfile );
                    printf( "adapt: Program failed.  Exiting.\n" );
                    exit( EXIT_FAILURE );

               }    /* if ( ferror( infile ) ) */

               /* See if anything went wrong with the temporary file. */

               if ( ferror( outfile ) )
               {
                    snprintf( str, len, "\
adapt: Failed to copy all of the data to the the temporary file \"%s\".\n",
                              TEMP_FILE );
                    close_files( str, save_errno, argv[ count ],
                                 &infile, &outfile );

                    /* Remove the temporary file. */

                    errno = 0;
                    ret = unlink( TEMP_FILE );
                    save_errno = errno;
                    if ( ret != 0 )
                    {
                         printf( "\
adapt: Failed to remove the temporary file \"%s\".\n", TEMP_FILE );
                         if ( save_errno != 0 )
                         {
                              printf( "adapt: Error: %s.\n",
                                      strerror( save_errno ) );
                         }
                         printf( "\
adapt: Catastrophic failure.  Aborting.\n" );
                         abort();  /* Crash and burn. */
                    }

                    printf( "adapt: Program failed.  Exiting.\n" );
                    exit( EXIT_FAILURE );

               }    /* if ( ferror( outfile ) ) */

               /*

                    At this point we have copied all the remaining
                    data from the input file to the temporary file.
                    The temporary file's buffer needs to be written
                    out so it can be closed.  Then, after closing
                    the input file we need to remove the input file
                    so it can be replaced with the temporary file
                    by renaming the temporary file.  Then we can
                    allow the for() loop to move on to the next
                    input file, if any.  close_files() will write
                    out any remaining data in the temporary file's
                    buffer before attempting to close it.

               */

               close_files( NULL, 0, argv[ count ], &infile, &outfile );

               /* Remove the input file. */

               errno = 0;
               ret = unlink( argv[ count ] );
               save_errno = errno;
               if ( ret != 0 )
               {
                    printf( "\
adapt: Failed to remove the file \"%s\" so it could be replaced with\
 the temporary file \"%s\".\n", argv[ count ], TEMP_FILE );
                    if ( save_errno != 0 )
                    {
                         printf( "adapt: Error: %s.\n",
                                 strerror( save_errno ) );
                    }

                    /* Remove the temporary file and crash. */

                    remove_temp_file();  /* Does not return */

               }    /* if ( ret != 0 ) */

               /* Rename the temporary file to replace the input file. */

               errno = 0;
               ret = rename( TEMP_FILE, argv[ count ] );
               save_errno = errno;
               if ( ret != 0 )
               {
                    printf( "\
adapt: Failed to rename the temporary file \"%s\" to \"%s\".\n",
                            TEMP_FILE, argv[ count ] );
                    if ( save_errno != 0 )
                    {
                         printf( "adapt: Error: %s.\n",
                                 strerror( save_errno ) );
                    }
                    printf( "adapt: Program failed.  Exiting.\n" );
                    exit( EXIT_FAILURE );
               }

               /* Increment the number of files adapted. */

               num_adapted++;

               /* Provide a notification of success. */

               printf( "Adapted: %s\n", argv[ count ] );

               /* Loop around again to process any other files. */

          }    /* if ( infile == NULL ) */
     }    /* for( count = 1; count < argc; count++ ) */

     /* Tell the users how many files were adapted, if any. */

     if ( num_adapted == 0 )
     {
          if ( open_failed == 1 )
          {
               printf( "Failed to open 1 file for reading.\n" );
          }
          else if ( open_failed > 1 )
          {
               printf( "Failed to open %d files for reading.\n",
                       open_failed );
          }
          printf( "No files adapted.\n" );
          if ( open_failed != 0 )
          {
               printf( "adapt: Program failed.  Exiting.\n" );
               exit( EXIT_FAILURE );
          }
     }
     else if ( num_adapted == 1 )
     {
          if ( open_failed == 1 )
          {
               printf( "Failed to open 1 file for reading.\n" );
          }
          else if ( open_failed > 1 )
          {
               printf( "Failed to open %d files for reading.\n",
                       open_failed );
          }
          printf( "1 file adapted.\n" );
     }
     else
     {
          if ( open_failed == 1 )
          {
               printf( "Failed to open 1 file for reading.\n" );
          }
          else if ( open_failed > 1 )
          {
               printf( "Failed to open %d files for reading.\n",
                       open_failed );
          }
          printf( "%d files adapted.\n", num_adapted );
     }

     /* And we're finally done, at long last. */

     exit( EXIT_SUCCESS );
}

/*

     This function closes both of the open files, which may point
     to NULL if they are not open.  If 'reason' is not NULL it
     will be printed as an explanation for this action.  If
     'save_errno' is not zero it will print the error message
     associated with that error code.  'name' is the file name
     associated with 'infile'.  This function will only call
     remove_temp_file() if it is going to end the program,
     otherwise it will not.

*/

void close_files( const char *reason, int save_errno, const char *name,
                  FILE **infile, FILE **outfile )
{
     int ret;

     /* Print the reason and error message if applicable. */

     if ( reason != NULL )
     {
          printf( "%s", reason );
          if ( save_errno != 0 )
          {
               printf( "adapt: Error: %s.\n", strerror( save_errno ) );
          }
     }

     /* Close the input file. */

     if ( infile != NULL && *infile != NULL )
     {
          /* Clear any error flags. */

          if ( feof( *infile ) || ferror( *infile ) )
          {
               clearerr( *infile );  /* This will clear feof(). */
          }

          /* Try to close the input file. */

          errno = 0;
          ret = fclose( *infile );
          save_errno = errno;
          if ( ret != 0 )
          {
               if ( name != NULL )
               {
                    printf( "adapt: Failed to close the file \"%s\".\n",
                            name );
               }
               else
               {
                    printf( "adapt: Failed to close the input file.\n" );
               }
               if ( save_errno != 0 )
               {
                    printf( "adapt: Error: %s.\n",
                            strerror( save_errno ) );
               }

               /* Close the temporary file. */

               if ( outfile != NULL && *outfile != NULL )
               {
                    /* Clear any error flags. */

                    if ( ferror( *outfile ) )
                    {
                         clearerr( *outfile );
                    }

                    /* Write out any data in the file's buffer. */

                    errno = 0;
                    fflush( *outfile );
                    save_errno = errno;
                    if ( ferror( *outfile ) )
                    {
                         printf( "\
adapt: Failed to write out any remaining data in the buffer\
 to the temporary file \"%s\".\n", TEMP_FILE );
                         if ( save_errno != 0 )
                         {
                              printf( "adapt: Error: %s.\n",
                                      strerror( save_errno ) );
                         }
                         clearerr( *outfile );
                    }

                    /* Try to close the temporary file. */

                    errno = 0;
                    ret = fclose( *outfile );
                    save_errno = errno;
                    if ( ret != 0 )
                    {
                         printf( "\
adapt: Failed to close the temporary file \"%s\".\n", TEMP_FILE );
                         if ( save_errno != 0 )
                         {
                              printf( "adapt: Error: %s.\n",
                                      strerror( save_errno ) );
                         }
                         printf( "\
adapt: Catastrophic failure.  Aborting.\n" );
                         abort();  /* Crash and burn. */
                    }
                    *outfile = NULL;

                    /* Could not close the input file. */

                    remove_temp_file();  /* Does not return */

                    /*

                         At this point the program is
                         a burning heap of debris.

                    */

               }    /* if ( *outfile != NULL ) */
          }    /* if ( ret != 0 ) */
          *infile = NULL;
     }    /* if ( *infile != NULL ) */

     /* Close the temporary file. */

     if ( outfile != NULL && *outfile != NULL )
     {
          /* Clear any error flags. */

          if ( ferror( *outfile ) )
          {
               clearerr( *outfile );
          }

          /* Write out any data in the file's buffer. */

          errno = 0;
          fflush( *outfile );
          save_errno = errno;
          if ( ferror( *outfile ) )
          {
               printf( "\
adapt: Failed to write out any remaining data in the buffer\
 to the temporary file \"%s\".\n", TEMP_FILE );
               if ( save_errno != 0 )
               {
                    printf( "adapt: Error: %s.\n",
                            strerror( save_errno ) );
               }
               clearerr( *outfile );
          }

          /* Try to close the temporary file. */

          errno = 0;
          ret = fclose( *outfile );
          save_errno = errno;
          if ( ret != 0 )
          {
               printf( "\
adapt: Failed to close the temporary file \"%s\".\n", TEMP_FILE );
               if ( save_errno != 0 )
               {
                    printf( "adapt: Error: %s.\n",
                            strerror( save_errno ) );
               }
               printf( "adapt: Catastrophic failure.  Aborting.\n" );
               abort();  /* Crash and burn. */
          }
          *outfile = NULL;
     }

     return;
}


/*

     This function will remove the temporary file, if it exists, and
     end the program.  This function is only called if something went
     terribly wrong.  If 'reason' is not NULL it will be printed as
     an explanation for this action.  If 'save_errno' is not zero it
     will print the error message associated with that error code.

*/

_Noreturn void remove_temp_file( void )
{
     int ret, save_errno;
     struct stat stat_buffer;

     /* Find out if our temporary file is present. */

     errno = 0;
     ret = stat( TEMP_FILE, &stat_buffer );
     save_errno = errno;
     if ( ret == ( -1 ) )
     {
          if ( save_errno == ENOENT )  /* No such file or directory. */
          {
               printf( "\
adapt: The temporary file \"%s\" has not been found in\
 the current directory and therefore cannot be removed.\n", TEMP_FILE );
               printf( "adapt: Catastrophic failure.  Aborting.\n" );
               abort();
          }
     }

     /* Try to remove the temporary file. */

     errno = 0;
     ret = unlink( TEMP_FILE );
     save_errno = errno;
     if ( ret != 0 )
     {
          printf( "\
adapt: Failed to remove the temporary file \"%s\".\n", TEMP_FILE );
          if ( save_errno != 0 )
          {
               printf( "adapt: Error: %s.\n", strerror( save_errno ) );
          }
     }

     printf( "adapt: Catastrophic failure.  Aborting.\n" );
     abort();  /* Crash and burn. */
}

/* EOF adapt.c */
