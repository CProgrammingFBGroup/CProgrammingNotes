/*

     roll.c
     Rolls the dice.
     Written by Matthew Campbell on Tuesday June 25, 2019.

     Updated on Monday October 5, 2020 to use $HOME.

     If the HOME environment variable is defined and it does not point to
     an empty string then the data file will be $HOME/DATA_FILE as #defined
     below.  If HOME does not exist or points to an empty string then a
     data file will not be used.

*/

/* snprintf(3) needs this: */

#define _DEFAULT_SOURCE
#undef _BSD_SOURCE       /* gcc says using _BSD_SOURCE is deprecated. */

/* Include the necessary header files: */

#include <time.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

/* Make sure these are defined: */

#ifndef ENOENT
#define ENOENT 2
#endif

#ifndef EINVAL
#define EINVAL 22
#endif

/* This is the name of the data file to use in $HOME/: */

#define DATA_FILE ".rolldata"

#define USAGE "\
Usage: roll mdn | 1 <= m <= 10 & 2 <= n <= 25\n\
       For example, if you wanted to roll two six sided dice use 2d6.\n"

/* Function prototypes: */

int  close_data_file( FILE **fp, char *fname );
void make_random_seed( void );

/* Function definitions: */

int main( int argc, char **argv )
{
     FILE *fp;
     char buffer[ 12 ], *env, fname[ 1025 ];
     int array[ 10 ], count, len, m, n, num, ret, save_errno, total;
     size_t size;
     struct stat stat_buf;
     time_t seed;

     /* This must be set to the size of the buffer: */

     len = 12;

     /* Make sure we have just one command line argument. */

     if ( argc != 2 )
     {
          printf( "%s", USAGE );
          exit( EXIT_FAILURE );
     }

     /* Make sure we have a valid command line argument. */

     if ( sscanf( argv[ 1 ], "%dd%d", &m, &n ) != 2 )
     {
          printf( "%s", USAGE );
          exit( EXIT_FAILURE );
     }

     /* Make sure the values for m and n are within acceptable range. */

     if ( m < 1 || m > 10 )
     {
          printf( "roll: Invalid value for m: %d.\n", m );
          printf( "%s", USAGE );
          exit( EXIT_FAILURE );
     }

     if ( n < 2 || n > 25 )
     {
          printf( "roll: Invalid value for n: %d.\n", n );
          printf( "%s", USAGE );
          exit( EXIT_FAILURE );
     }

     /* See if the HOME environment variable is defined. */

     env = getenv( "HOME" );

     if ( env == NULL || env[ 0 ] == 0 )
     {
          fname[ 0 ] = 0;  /* Initialize to null. */
     }
     else  /* Determine the file name to store the seed data in. */
     {
          size = strlen( env );

          if ( ( size + strlen( DATA_FILE ) + 1 ) <= 1024 )
          {
               strcpy( fname, env );
               fname[ ( size++ ) ] = '/';
               strcpy( &fname[ size ], DATA_FILE );
               size += strlen( DATA_FILE );
               fname[ size ] = 0;
          }
          else
          {
               fname[ 0 ] = 0;  /* Initialize to null. */
          }

     }

     if ( fname[ 0 ] != 0 )
     {
          /* See if our data file exists. */

          errno = 0;
          ret = stat( fname, &stat_buf );
          save_errno = errno;

          if ( ret != 0 )
          {

               if ( save_errno != ENOENT )
               {
                    printf( "\
Something went wrong while looking for the data file \"%s\".\n", fname );

                    if ( save_errno != 0 )
                    {
                         printf( "Error: %s.\n", strerror( save_errno ) );
                    }

                    exit( EXIT_FAILURE );
               }

               /* The file does not exist so it will need to be created. */

               errno = 0;
               fp = fopen( fname, "w" );
               save_errno = errno;

               if ( fp == NULL )
               {
                    printf( "Failed to create the data file \"%s\".\n",
                            fname );

                    if ( save_errno != 0 )
                    {
                         printf( "Error: %s.\n", strerror( save_errno ) );
                    }

                    exit( EXIT_FAILURE );
               }

               /* We will need a random number seed. */

               make_random_seed();
          }
          else  /* ret == 0 */
          {
               /* Open the data file. */

               errno = 0;
               fp = fopen( fname, "r" );
               save_errno = errno;

               if ( fp == NULL )
               {
                    printf( "Failed to open the data file \"%s\".\n",
                            fname );

                    if ( save_errno != 0 )
                    {
                         printf( "Error: %s.\n", strerror( save_errno ) );
                    }

                    exit( EXIT_FAILURE );
               }

               /* Read the number stored in the data file. */

               errno = 0;
               ret = fscanf( fp, "%d", &num );
               save_errno = errno;

               if ( ret == ( -1 ) )
               {
                    printf( "\
Something went wrong while reading from the data file \"%s\".\n\
Removing it...\n", fname );

                    if ( save_errno != 0 )
                    {
                         printf( "Error: %s.\n", strerror( save_errno ) );
                    }

                    /* Close the data file. */

                    ret = close_data_file( &fp, fname );

                    if ( ret != 0 )
                    {
                         abort();  /* Crash and burn. */
                    }

                    /* Start fresh with a new seed and a blank data file. */

                    errno = 0;
                    fp = fopen( fname, "w" );
                    save_errno = errno;

                    if ( fp == NULL )
                    {
                         printf( "Failed to reset the data file \"%s\".\n",
                                 fname );

                         if ( save_errno != 0 )
                         {
                              printf( "Error: %s.\n",
                                      strerror( save_errno ) );
                         }

                         exit( EXIT_FAILURE );
                    }

                    /* We will need a random number seed. */

                    make_random_seed();
               }
               else if ( ret == 0 )
               {
                    printf( "\
The data file \"%s\" is corrupt.\nRemoving it...\n", fname );
                    ret = close_data_file( &fp, fname );

                    if ( ret != 0 )
                    {
                         abort();  /* Crash and burn. */
                    }

                    /* Start fresh with a new seed and a blank data file. */

                    errno = 0;
                    fp = fopen( fname, "w" );
                    save_errno = errno;

                    if ( fp == NULL )
                    {
                         printf( "Failed to reset the data file \"%s\".\n",
                                 fname );

                         if ( save_errno != 0 )
                         {
                              printf( "Error: %s.\n",
                                      strerror( save_errno ) );
                         }

                         exit( EXIT_FAILURE );
                    }

                    /* We will need a random number seed. */

                    make_random_seed();
               }
               else  /* ret == 1 */
               {
                    /* Initialize the random number generator. */

                    seed = ( time_t )num;
                    srand( ( unsigned int )seed );

                    /*

                         I believe it's easier to close and open the
                         data file instead of rewinding it and then
                         truncating it to a length of zero bytes.

                    */

                    ret = close_data_file( &fp, fname );

                    if ( ret != 0 )
                    {
                         abort();
                    }

                    /* Open the data file for writing. */

                    errno = 0;
                    fp = fopen( fname, "w" );
                    save_errno = errno;

                    if ( fp == NULL )
                    {
                         printf( "Failed to reset the data file \"%s\".\n",
                                 fname );

                         if ( save_errno != 0 )
                         {
                              printf( "Error: %s.\n",
                                      strerror( save_errno ) );
                         }

                         exit( EXIT_FAILURE );
                    }

               }    /* if ( ret == ( -1 ) ) */

          }    /* if ( ret != 0 ) */

     }
     else  /* fname[ 0 ] == 0 */
     {
          fp = NULL;  /* Initialize to NULL. */

          /* We will need a random number seed. */

          make_random_seed();

     }    /* if ( fname[ 0 ] != 0 ) */

     /*

          At this point the random number generator is initialized,
          the data file has a length of zero bytes and it is open
          for writing, unless a data file is not being used.

     */

     total = 0;

     for( count = 0; count < m; count++ )
     {
          num = rand();
          array[ count ] = ( ( num >> 16 ) % n ) + 1;
          total = total + array[ count ];
     }

     if ( fp != NULL )
     {
          /* Save the last number generated for the next use. */

          errno = 0;
          count = snprintf( buffer, len, "%d\n", num );
          save_errno = errno;

          if ( count < 2 )
          {
               printf( "Failed to save the last number generated.\n" );

               if ( save_errno != 0 )
               {
                    printf( "Error: %s.\n", strerror( save_errno ) );
               }

               ret = close_data_file( &fp, fname );

               if ( ret != 0 )
               {
                    abort();
               }

               exit( EXIT_FAILURE );
          }

          errno = 0;
          ret = fprintf( fp, "%s", buffer );
          save_errno = errno;

          if ( count != ret )
          {
               printf( "Failed to save the last number generated.\n" );

               if ( save_errno != 0 )
               {
                    printf( "Error: %s.\n", strerror( save_errno ) );
               }

               ret = close_data_file( &fp, fname );

               if ( ret != 0 )
               {
                    abort();
               }

               exit( EXIT_FAILURE );
          }

          /* Write out any data in the buffer. */

          errno = 0;
          fflush( fp );
          save_errno = errno;

          if ( ferror( fp ) )
          {
               printf( "Failed to save the last number generated.\n" );

               if ( save_errno != 0 )
               {
                    printf( "Error: %s.\n", strerror( save_errno ) );
               }

               ret = close_data_file( &fp, fname );

               if ( ret != 0 )
               {
                    abort();
               }

               exit( EXIT_FAILURE );
          }

          /* Close the data file. */

          ret = close_data_file( &fp, fname );

          if ( ret != 0 )
          {
               abort();
          }

     }    /* if ( fp != NULL ) */

     /* Display the results. */

     printf( "Total: %d.  Rolled: ({ ", total );
     num = m - 1;

     for( count = 0; count < num; count++ )
     {
          printf( "%d, ", array[ count ] );
     }

     printf( "%d })\n", array[ num ] );

     /* And we're done. */

     exit( EXIT_SUCCESS );
}

/*

     Closes the data file.
     Returns 0 for success, or -1 if an error occurred.

*/

int close_data_file( FILE **fp, char *fname )
{
     int ret, save_errno;

     /* See if we have something to work with. */

     if ( fp == NULL || *fp == NULL )
     {
          printf( "Null file pointer passed to close_data_file().\n" );
          errno = EINVAL;
          return ( -1 );
     }

     /* Clear any error flag. */

     if ( ferror( *fp ) )
     {
          clearerr( *fp );
     }

     /* Close the data file. */

     errno = 0;
     ret = fclose( *fp );
     save_errno = errno;

     if ( ret != 0 )
     {
          printf( "Failed to close the data file \"%s\".\n", fname );

          if ( save_errno != 0 )
          {
               printf( "Error: %s.\n", strerror( save_errno ) );
          }

          errno = save_errno;
          return ( -1 );
     }

     *fp = NULL;
     errno = save_errno;
     return 0;
}

/*

     Generates a random seed from the current time and
     uses it to initialize the random number generator.

*/

void make_random_seed( void )
{
     int save_errno;
     time_t seed;

     errno = 0;
     seed = time( NULL );
     save_errno = errno;

     if ( seed == ( time_t )( -1 ) )
     {
          printf( "Failed to generate a random seed.\n" );

          if ( save_errno != 0 )
          {
               printf( "Error: %s.\n", strerror( save_errno ) );
          }

          exit( EXIT_FAILURE );
     }

     srand( ( unsigned int )seed );

     return;
}

/* EOF roll.c */
