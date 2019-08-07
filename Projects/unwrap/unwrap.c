/*

     unwrap.c
     Joins multiple lines in each paragraph into a single line.
     Written by Matthew Campbell on Wednesday July 31, 2019.

*/

/* Include what we need: */

/* snprintf(3) needs this: */

#define _BSD_SOURCE

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Define our USAGE statement: */

#define USAGE "\
Usage: unwrap [input file] [output file]\n\
Joins multiple lines in each paragraph into a single line.\n"

/* Make sure these are defined: */

#ifndef EFAULT
#define EFAULT 14
#endif

#ifndef EINVAL
#define EINVAL 22
#endif

/* Function prototypes: */

int close_files( FILE *infile, FILE *outfile, char *infile_name,
                 char *outfile_name );

int write_previous_byte( int byte, FILE *infile, FILE *outfile,
                         char *infile_name, char *outfile_name );

/* Function definitions: */

int main( int argc, char **argv )
{
     FILE *infile, *outfile;
     static char infile_name[ 1036 ], outfile_name[ 1036 ];
     int count, current, last, ret, save_errno, spaces, was_newline;

     /* Make sure we have received the correct arguments. */

     if ( argc > 3 )
     {
          fprintf( stderr, "%s", USAGE );
          exit( EXIT_FAILURE );
     }

     if ( argc == 2 )
     {
          /* Did the user ask for help? */

          if ( strcmp( argv[ 1 ], "-h" ) == 0 ||
               strcmp( argv[ 1 ], "--help" ) == 0 )
          {
               fprintf( stderr, "%s", USAGE );
               exit( EXIT_SUCCESS );
          }

          /* Prepare the infile_name string. */

          errno = 0;
          ret = snprintf( infile_name, 1036, "the file \"%s\"",
                          argv[ 1 ] );
          if ( ret == ( -1 ) )
          {
               save_errno = errno;
               fprintf( stderr, "\
unwrap: Failed to prepare the infile_name string.\n" );
               if ( save_errno != 0 )
               {
                    fprintf( stderr, "unwrap: Error: %s.\n",
                             strerror( save_errno ) );
               }
               exit( EXIT_FAILURE );
          }

          /* Open the input file stream. */

          errno = 0;
          infile = fopen( argv[ 1 ], "r" );
          if ( infile == NULL )
          {
               fprintf( stderr, "\
unwrap: Failed to open \"%s\" for reading.\n", argv[ 1 ] );
               exit( EXIT_FAILURE );
          }

          /* Use stdout for the output file stream. */

          outfile = stdout;
          strcpy( outfile_name, "stdout" );
     }
     else if ( argc == 1 )
     {
          infile = stdin;
          outfile = stdout;
          strcpy( infile_name, "stdin" );
          strcpy( outfile_name, "stdout" );
     }
     else  /* argc == 3, open both files. */
     {
          /* Prepare the infile_name string. */

          errno = 0;
          ret = snprintf( infile_name, 1036, "the file \"%s\"",
                          argv[ 1 ] );
          if ( ret == ( -1 ) )
          {
               save_errno = errno;
               fprintf( stderr, "\
unwrap: Failed to prepare the infile_name string.\n" );
               if ( save_errno != 0 )
               {
                    fprintf( stderr, "unwrap: Error: %s.\n",
                             strerror( save_errno ) );
               }
               exit( EXIT_FAILURE );
          }

          /* Prepare the outfile_name string. */

          errno = 0;
          ret = snprintf( outfile_name, 1036, "the file \"%s\"",
                          argv[ 2 ] );
          if ( ret == ( -1 ) )
          {
               save_errno = errno;
               fprintf( stderr, "\
unwrap: Failed to prepare the outfile_name string.\n" );
               if ( save_errno != 0 )
               {
                    fprintf( stderr, "unwrap: Error: %s.\n",
                             strerror( save_errno ) );
               }
               exit( EXIT_FAILURE );
          }

          /* Open the input file stream. */

          errno = 0;
          infile = fopen( argv[ 1 ], "r" );
          if ( infile == NULL )
          {
               save_errno = errno;
               fprintf( stderr, "\
unwrap: Failed to open \"%s\" for reading.\n", argv[ 1 ] );
               if ( save_errno != 0 )
               {
                    fprintf( stderr, "unwrap: Error: %s.\n",
                             strerror( save_errno ) );
               }
               exit( EXIT_FAILURE );
          }

          /* Open the output file stream. */

          errno = 0;
          outfile = fopen( argv[ 2 ], "w" );
          if ( outfile == NULL )
          {
               save_errno = errno;
               fprintf( stderr, "\
unwrap: Failed to open \"%s\" for writing.\n", argv[ 2 ] );
               if ( save_errno != 0 )
               {
                    fprintf( stderr, "unwrap: Error: %s.\n",
                             strerror( save_errno ) );
               }

               /* Close the input file stream. */

               errno = 0;
               ret = fclose( infile );
               if ( ret != 0 )
               {
                    save_errno = errno;
                    fprintf( stderr, "\
unwrap: Failed to close the file \"%s\".\n", argv[ 1 ] );
                    if ( save_errno != 0 )
                    {
                         fprintf( stderr, "unwrap: Error: %s.\n",
                                  strerror( save_errno ) );
                    }
               }
               exit( EXIT_FAILURE );

          }    /* if ( outfile == NULL ) */

     }    /* if ( argc == 2 ) */

     /* At this point, both file streams are available. */

     /* Initialize: */

     spaces = was_newline = 0;
     current = last = ( -1 );

     /* Go to work: */

     while( !feof( infile ) && !ferror( infile ) &&
            !ferror( outfile ) && ( current = fgetc( infile ) ) != EOF )
     {
          if ( current != '\n' )
          {
               /*

                    If a poorly formed text file that does not end with
                    a newline character is used as the input file and
                    the file ends with a newline character followed by
                    one or more space characters then the last newline
                    would be converted to a space which would disrupt
                    the required reciprocity between wrap and unwrap.

                    As a result, we must keep track of this condition
                    while using nonseekable file streams and without
                    buffering more than two bytes.  We need to keep
                    track of the number of space characters after the
                    newline character that was converted to a space,
                    but has yet to be written to the output file stream.

               */

               if ( current == 32 )  /* Space */
               {
                    if ( spaces > 0 )
                    {
                         spaces++;
                    }
                    else if ( was_newline == 1 )
                    {
                         spaces = 1;
                    }
               }
               else
               {
                    if ( spaces > 0 )
                    {
                         for( count = spaces; count >= 0; count-- )
                         {
                              ret = write_previous_byte( 32, infile,
                                                         outfile,
                                                         infile_name,
                                                         outfile_name );
                              if ( ret != 0 )
                              {
                                   save_errno = errno;
                                   if ( save_errno != 0 )
                                   {
                                        fprintf( stderr, "\
unwrap: Something went wrong when calling write_previous_byte().\n" );
                                        fprintf( stderr,
                                                 "unwrap: Error: %s.\n",
                                                 strerror( save_errno ) );
                                        ret = close_files( infile, outfile,
                                                           infile_name, 
                                                           outfile_name );
                                        if ( ret != 0 )
                                        {
                                             if ( errno != 0 )
                                             {
                                                  save_errno = errno;
                                                  fprintf( stderr, "\
unwrap: Something went wrong when trying to close the file streams.\n" );
                                                  fprintf( stderr, "\
unwrap: Error: %s.\n", strerror( save_errno ) );
                                             }
                                        }
                                   }    /* if ( save_errno != 0 ) */
                                   exit( EXIT_FAILURE );
                              }    /* if ( ret != 0 ) */
                         }    /* for( count = spaces; count >= 0;
                                      count-- ) */
                         /*

                              Keep track of the fact that we
                              wrote out all of the pending
                              data to the output file stream.

                         */

                         last = ( -1 );
                         spaces = 0;

                    }    /* if ( spaces > 0 ) */
               }    /* if ( current == 32 ) */

               was_newline = 0;
               if ( last != ( -1 ) && spaces == 0 )
               {
                    ret = write_previous_byte( last, infile, outfile,
                                               infile_name, outfile_name );
                    if ( ret != 0 )
                    {
                         save_errno = errno;
                         if ( save_errno != 0 )
                         {
                              fprintf( stderr, "\
unwrap: Something went wrong when calling write_previous_byte().\n" );
                              fprintf( stderr, "unwrap: Error: %s.\n",
                                       strerror( save_errno ) );
                              ret = close_files( infile, outfile,
                                                 infile_name, 
                                                 outfile_name );
                              if ( ret != 0 )
                              {
                                   if ( errno != 0 )
                                   {
                                        save_errno = errno;
                                        fprintf( stderr, "\
unwrap: Something went wrong when trying to close the file streams.\n" );
                                        fprintf( stderr, "\
unwrap: Error: %s.\n", strerror( save_errno ) );
                                   }
                              }
                         }    /* if ( save_errno != 0 ) */
                         exit( EXIT_FAILURE );
                    }    /* if ( ret != 0 ) */
               }    /* if ( last != ( -1 ) && spaces == 0 ) */

               last = current;
          }
          else  /* current == '\n' */
          {
               /* Write out any pending spaces first. */

               if ( spaces > 0 )
               {
                    /* Write out the newline character. */

                    ret = write_previous_byte( '\n', infile, outfile,
                                               infile_name,
                                               outfile_name );
                    if ( ret != 0 )
                    {
                         save_errno = errno;
                         if ( save_errno != 0 )
                         {
                              fprintf( stderr, "\
unwrap: Something went wrong when calling write_previous_byte().\n" );
                              fprintf( stderr,
                                       "unwrap: Error: %s.\n",
                                       strerror( save_errno ) );
                              ret = close_files( infile, outfile,
                                                 infile_name, 
                                                 outfile_name );
                              if ( ret != 0 )
                              {
                                   if ( errno != 0 )
                                   {
                                        save_errno = errno;
                                        fprintf( stderr, "\
unwrap: Something went wrong when trying to close the file streams.\n" );
                                        fprintf( stderr, "\
unwrap: Error: %s.\n", strerror( save_errno ) );
                                   }
                              }
                         }    /* if ( save_errno != 0 ) */
                         exit( EXIT_FAILURE );
                    }    /* if ( ret != 0 ) */

                    /* Now write out most of the pending spaces. */

                    for( count = spaces; count > 0; count-- )
                    {
                         ret = write_previous_byte( 32, infile, outfile,
                                                    infile_name,
                                                    outfile_name );
                         if ( ret != 0 )
                         {
                              save_errno = errno;
                              if ( save_errno != 0 )
                              {
                                   fprintf( stderr, "\
unwrap: Something went wrong when calling write_previous_byte().\n" );
                                   fprintf( stderr,
                                            "unwrap: Error: %s.\n",
                                            strerror( save_errno ) );
                                   ret = close_files( infile, outfile,
                                                      infile_name, 
                                                      outfile_name );
                                   if ( ret != 0 )
                                   {
                                        if ( errno != 0 )
                                        {
                                             save_errno = errno;
                                             fprintf( stderr, "\
unwrap: Something went wrong when trying to close the file streams.\n" );
                                             fprintf( stderr, "\
unwrap: Error: %s.\n", strerror( save_errno ) );
                                        }
                                   }
                              }    /* if ( save_errno != 0 ) */
                              exit( EXIT_FAILURE );
                         }    /* if ( ret != 0 ) */
                    }    /* for( count = spaces; count > 0; count-- ) */

                    /*

                         Keep track of the fact that we wrote
                         out all but one of the pending spaces
                         to the output file stream.

                    */

                    last = 32;
                    spaces = 0;

               }    /* if ( spaces > 0 ) */

               if ( last != '\n' && last != ( -1 ) )
               {
                    current = 32;  /* Space */
                    was_newline = 1;
               }

               if ( last != ( -1 ) )
               {
                    ret = write_previous_byte( last, infile, outfile,
                                               infile_name, outfile_name );
                    if ( ret != 0 )
                    {
                         save_errno = errno;
                         if ( save_errno != 0 )
                         {
                              fprintf( stderr, "\
unwrap: Something went wrong when calling write_previous_byte().\n" );
                              fprintf( stderr, "unwrap: Error: %s.\n",
                                       strerror( save_errno ) );
                              ret = close_files( infile, outfile,
                                                 infile_name, 
                                                 outfile_name );
                              if ( ret != 0 )
                              {
                                   if ( errno != 0 )
                                   {
                                        save_errno = errno;
                                        fprintf( stderr, "\
unwrap: Something went wrong when trying to close the file streams.\n" );
                                        fprintf( stderr, "\
unwrap: Error: %s.\n", strerror( save_errno ) );
                                   }
                              }
                         }    /* if ( save_errno != 0 ) */
                         exit( EXIT_FAILURE );
                    }    /* if ( ret != 0 ) */
               }    /* if ( last != ( -1 ) ) */

               last = current;
          }    /* if ( current != '\n' ) */
     }    /* while( !feof( infile ) && !ferror( infile ) &&
            !ferror( outfile ) && ( current = fgetc( infile ) ) != EOF ) */

     /* Did anything go wrong? */

     if ( ferror( outfile ) )
     {
          ret = close_files( infile, outfile, infile_name, outfile_name );
          if ( ret != 0 )
          {
               if ( errno != 0 )
               {
                    save_errno = errno;
                    fprintf( stderr, "\
unwrap: Something went wrong when trying to close the file streams.\n" );
                    fprintf( stderr, "\
unwrap: Error: %s.\n", strerror( save_errno ) );
               }
               exit( EXIT_FAILURE );
          }
     }    /* if ( ferror( outfile ) ) */

     if ( !feof( infile ) && ferror( infile ) )
     {
          ret = close_files( infile, outfile, infile_name, outfile_name );
          if ( ret != 0 )
          {
               if ( errno != 0 )
               {
                    save_errno = errno;
                    fprintf( stderr, "\
unwrap: Something went wrong when trying to close the file streams.\n" );
                    fprintf( stderr, "\
unwrap: Error: %s.\n", strerror( save_errno ) );
               }
               exit( EXIT_FAILURE );
          }
     }    /* if ( !feof( infile ) && ferror( infile ) ) */

     /*

          If nothing went wrong so far then we need
          to write out the last newline character.

     */

     if ( was_newline == 1 )
     {
          ret = write_previous_byte( '\n', infile, outfile, infile_name,
                                     outfile_name );
          if ( ret != 0 )
          {
               if ( errno != 0 )
               {
                    save_errno = errno;
                    fprintf( stderr, "\
unwrap: Something went wrong when calling write_previous_byte().\n" );
                    fprintf( stderr, "unwrap: Error: %s.\n",
                             strerror( save_errno ) );
                    ret = close_files( infile, outfile, infile_name,
                                       outfile_name );
                    if ( ret != 0 )
                    {
                         if ( errno != 0 )
                         {
                              save_errno = errno;
                              fprintf( stderr, "\
unwrap: Something went wrong when trying to close the file streams.\n" );
                              fprintf( stderr, "\
unwrap: Error: %s.\n", strerror( save_errno ) );
                         }
                    }
               }    /* if ( save_errno != 0 ) */
               exit( EXIT_FAILURE );
          }    /* if ( ret != 0 ) */
     }
     else if ( spaces > 0 )
     {
          /* Write out the newline character. */

          ret = write_previous_byte( '\n', infile, outfile, infile_name,
                                     outfile_name );
          if ( ret != 0 )
          {
               save_errno = errno;
               if ( save_errno != 0 )
               {
                    fprintf( stderr, "\
unwrap: Something went wrong when calling write_previous_byte().\n" );
                    fprintf( stderr, "unwrap: Error: %s.\n",
                             strerror( save_errno ) );
                    ret = close_files( infile, outfile, infile_name,
                                       outfile_name );
                    if ( ret != 0 )
                    {
                         if ( errno != 0 )
                         {
                              save_errno = errno;
                              fprintf( stderr, "\
unwrap: Something went wrong when trying to close the file streams.\n" );
                              fprintf( stderr, "\
unwrap: Error: %s.\n", strerror( save_errno ) );
                         }
                    }
               }    /* if ( save_errno != 0 ) */
               exit( EXIT_FAILURE );
          }    /* if ( ret != 0 ) */

          /* Now write out all of the pending spaces. */

          for( count = spaces; count > 0; count-- )
          {
               ret = write_previous_byte( 32, infile, outfile, infile_name,
                                          outfile_name );
               if ( ret != 0 )
               {
                    save_errno = errno;
                    if ( save_errno != 0 )
                    {
                         fprintf( stderr, "\
unwrap: Something went wrong when calling write_previous_byte().\n" );
                         fprintf( stderr, "unwrap: Error: %s.\n",
                                  strerror( save_errno ) );
                         ret = close_files( infile, outfile, infile_name,
                                            outfile_name );
                         if ( ret != 0 )
                         {
                              if ( errno != 0 )
                              {
                                   save_errno = errno;
                                   fprintf( stderr, "\
unwrap: Something went wrong when trying to close the file streams.\n" );
                                   fprintf( stderr, "\
unwrap: Error: %s.\n", strerror( save_errno ) );
                              }
                         }
                    }    /* if ( save_errno != 0 ) */
                    exit( EXIT_FAILURE );
               }    /* if ( ret != 0 ) */
          }    /* for( count = spaces; count > 0; count-- ) */
     }

     /* Now we need to close the file streams. */

     ret = close_files( infile, outfile, infile_name, outfile_name );
     if ( ret != 0 )
     {
          if ( errno != 0 )
          {
               save_errno = errno;
               fprintf( stderr, "\
unwrap: Something went wrong when trying to close the file streams.\n" );
               fprintf( stderr, "\
unwrap: Error: %s.\n", strerror( save_errno ) );
          }
          exit( EXIT_FAILURE );
     }

     /* And we're done, at long last. */

     exit( EXIT_SUCCESS );
}

/*

     This function closes the input and output file streams.
     Returns 0 for success, or 1 if an error occured.

*/

int close_files( FILE *infile, FILE *outfile, char *infile_name,
                 char *outfile_name )
{
     int errors, ret, save_errno;

     /* Make sure we have received valid parameters. */

     if ( infile == NULL )
     {
          errno = EFAULT;
          return 1;
     }
     if ( outfile == NULL )
     {
          errno = EFAULT;
          return 1;
     }
     if ( infile_name == NULL )
     {
          errno = EFAULT;
          return 1;
     }
     if ( infile_name[ 0 ] == 0 )
     {
          errno = EINVAL;
          return 1;
     }
     if ( outfile == NULL )
     {
          errno = EFAULT;
          return 1;
     }
     if ( outfile_name[ 0 ] == 0 )
     {
          errno = EINVAL;
          return 1;
     }

     errors = 0;  /* Initialize */

     /* Flush any remaining buffered data. */

     if ( ferror( outfile ) )
     {
          clearerr( outfile );
     }

     errno = 0;
     fflush( outfile );
     if ( ferror( outfile ) )
     {
          save_errno = errno;
          errors++;
          fprintf( stderr, "\
unwrap: Failed to write out any remaining buffered data.\n" );
          if ( save_errno != 0 )
          {
               fprintf( stderr, "unwrap: Error: %s.\n",
                        strerror( save_errno ) );
          }
          clearerr( outfile );
     }

     /* Close the output file stream. */

     errno = 0;
     ret = fclose( outfile );
     if ( ret != 0 )
     {
          save_errno = errno;
          errors++;
          fprintf( stderr, "unwrap: Failed to close %s.\n", outfile_name );
          if ( save_errno != 0 )
          {
               fprintf( stderr, "unwrap: Error: %s.\n",
                        strerror( save_errno ) );
          }
     }

     /* Close the input file. */

     errno = 0;
     ret = fclose( infile );
     if ( ret != 0 )
     {
          save_errno = errno;
          errors++;
          fprintf( stderr, "unwrap: Failed to close %s.\n", infile_name );
          if ( save_errno != 0 )
          {
               fprintf( stderr, "unwrap: Error: %s.\n",
                        strerror( save_errno ) );
          }
     }

     errno = 0;
     if ( errors != 0 )
     {
          return 1;
     }
     else
     {
          return 0;
     }
}

/*

     This function writes the previous byte read.
     Returns 0 for success, or 1 if an error occured.

*/

int write_previous_byte( int byte, FILE *infile, FILE *outfile,
                         char *infile_name, char *outfile_name )
{
     int ret, save_errno;

     /* Make sure we have received valid parameters. */

     /*

          Maybe it would have been better to simply copy and
          paste this code in main() so all of this wouldn't
          have to be checked every time the program wrote a
          single byte to the output file stream.

     */

     if ( byte < 0 || byte > 255 )
     {
          errno = EINVAL;
          return 1;
     }
     if ( infile == NULL )
     {
          errno = EFAULT;
          return 1;
     }
     if ( outfile == NULL )
     {
          errno = EFAULT;
          return 1;
     }
     if ( infile_name == NULL )
     {
          errno = EFAULT;
          return 1;
     }
     if ( infile_name[ 0 ] == 0 )
     {
          errno = EINVAL;
          return 1;
     }
     if ( outfile == NULL )
     {
          errno = EFAULT;
          return 1;
     }
     if ( outfile_name[ 0 ] == 0 )
     {
          errno = EINVAL;
          return 1;
     }

     /* Write the previous byte to the output file stream. */

     errno = 0;
     ret = fputc( byte, outfile );

     /* Did it work? */

     if ( ret == EOF )
     {
          save_errno = errno;
          fprintf( stderr, "\
unwrap: Something went wrong when writing to %s.\n", outfile_name );
          if ( save_errno != 0 )
          {
               fprintf( stderr, "unwrap: Error: %s.\n",
                        strerror( save_errno ) );
          }

          ret = close_files( infile, outfile, infile_name, outfile_name );
          if ( ret != 0 )
          {
               save_errno = errno;
               if ( save_errno != 0 )
               {
                    fprintf( stderr, "\
unwrap: Something went wrong when trying to close the file streams.\n" );
                    fprintf( stderr, "unwrap: Error: %s.\n",
                             strerror( save_errno ) );
               }
               errno = 0;
               return 1;
          }

     }    /* if ( ret == EOF ) */

     errno = 0;
     return 0;
}

/* EOF unwrap.c */
