/*

     prime.c
     Finds prime numbers using an iterative method.

     This program was originally intended to be provided as a good
     example of a program for finding prime numbers.  Unfortunately
     it has turned into a bad case of stone soup.  It started out
     simple.  Then I started adding features like copying the
     program's output to a file and giving the user other options.
     Now it has turned into this huge program.

     The important part is the function is_prime().

     Written by Matthew Campbell for the C Programming group.  (12-13-17)

     The date is when I started writing this program.  It took about two
     and a half days.  I just couldn't stop tinkering with it.

     The function scanf() was replaced with getchar()
     when asking for a yes or no answer.  (1-4-18)

     Modified to be able to write to a
     file when running as an app. (8-19-18)

     Modified to allow the program to use its directory in /data or
     its data directory in internal storage Android/data/ to store
     the output file. (9-29-18)

*/

#include <math.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/*

     Define DEBUG if you want to see each number
     that is being tested to see if it is a prime number.

*/

#undef DEBUG

/*

     Define NEGATIVE_IS_NOT_PRIME if you want strict sign checking enabled.

*/

#define NEGATIVE_IS_NOT_PRIME

/*

     Some people may want 1 listed with any prime numbrrs found.
     Define LIST_ONE_AS_PRIME if you want this, otherwise don't.

*/

#define LIST_ONE_AS_PRIME

/*

     Define COPY_OUTPUT_TO_FILE if you want a copy of the output
     sent to a file.  If this program is used as an APK app file
     then the user will not be able to redirect the output to a
     text file when running the program.

*/

#define COPY_OUTPUT_TO_FILE

#ifdef COPY_OUTPUT_TO_FILE

#define BUFFER_SIZE 1026

#include <sys/stat.h>
#include <sys/types.h>

#endif

/*

     Define RUN_AS_APP if this program wil be
     installed and run as a stand alone app.

*/

#define RUN_AS_APP

/*

     Define USE_DATA if you want the output file to be
     written to the /data/data/app.prime/files/ directory.

*/

#undef USE_DATA

int is_prime( int num );

#ifdef COPY_OUTPUT_TO_FILE

int check_error( FILE *fp, char *name, int add_line );

#ifdef RUN_AS_APP

#ifdef USE_DATA

int setup_directory( char *program_name );

#endif

#endif

#endif

int main( int argc, char **argv )
{
     char letter;
     int ret, show_numbers, show_total, start, stop;
     register int count, total;

#ifndef DEBUG

     int one_per_line;

#endif

#ifdef COPY_OUTPUT_TO_FILE

     FILE *fp;
     char name[ BUFFER_SIZE ];  /* How long can file names be in Linux? */
     int use_file, prevent_file = 0;
     struct stat status;

#endif

     printf( "\
This program finds prime numbers using an iterative method.\n" );

#ifdef DEBUG

#ifdef NEGATIVE_IS_NOT_PRIME

     printf( "\nStrict sign checking is enabled.\n" );

#else

     printf( "\nStrict sign checking is not enabled.\n" );

#endif

#ifdef LIST_ONE_AS_PRIME

     printf( "Listing 1 with any prime numbers found.\n" );

#else

     printf( "Not listing 1 with any prime numbers found.\n" );

#endif

#endif

     if ( argc != 1 )
     {
          fprintf( stderr, "\
Prime does not accept command line arguments.\n\n" );
          return 1;
     }

#ifdef COPY_OUTPUT_TO_FILE

#ifdef RUN_AS_APP

#ifdef USE_DATA

     if ( setup_directory( argv[ 0 ] ) != 0 )
     {
          fprintf( stderr, "\
Failed to prepare the directory to save the output file.\n" );
          prevent_file = 1;
     }

#endif

#endif

#endif

/*

     Now we gather our input from the user.
     First we need a starting point.

*/

     do
     {
          printf( "\nWhat number would you like to start at? > " );
          fflush( stdout );
          ret = scanf( "%d", &start );
          if  ( ret != 1 || start < 1 )
          {
               ret = start = 0;
               printf( "\nSorry, that is not a valid entry.\n" );
               printf( "\
Please select an integer number greater than zero.\n" );
          }
     }    while( ret != 1 );

/*

     And then a stopping point so we know when we're done.

*/

     do
     {
          printf( "\nWhat number would you like to stop at? > " );
          fflush( stdout );
          ret = scanf( "%d", &stop );
          if  ( ret != 1 || stop < 1 )
          {
               ret = stop = 0;
               printf( "\nSorry, that is not a valid entry.\n" );
               printf( "\
Please select an integer number greater than zero.\n" );
          }
          if  ( ret == 1 && stop < start )
          {
               ret = stop = 0;
               printf( "\nSorry, that is not a valid entry.\n" );
               printf( "\
The number you stop at must not be less than the number you start at.\n" );
          }
     }    while( ret != 1 );

/*

     Now we need to know if the user wants each
     prime number to be displayed on its own line.

*/

#ifndef DEBUG

     one_per_line = 0;
     do
     {
          printf( "\n\
Would you like to have each prime number\n\
displayed on its own line? (y/n) > " );
          fflush( stdout );
          letter = ( char )getchar();

          while( letter == '\n' )
          {
               letter = ( char )getchar();
          }
          if ( letter == 'Y' )
          {
               letter = 'y';
          }
          else if ( letter == 'N' )
          {
               letter = 'n';
          }
          if ( letter != 'y' && letter != 'n' )
          {
               printf( "\nSorry, that is not a valid entry.\n" );
               printf( "Please answer with y or n.\n" );
               ret = 0;
          }
          else
          {
               ret = 1;
          }
     }    while( ret != 1 );
     if ( letter == 'y' )
     {
          one_per_line = 1;
     }
     letter = 0;

#endif

#ifdef COPY_OUTPUT_TO_FILE

     if ( prevent_file == 0 )
     {
          do
          {
               printf( "\n\
Would you like to copy the program's\n\
output to a text file? (y/n) > " );
               fflush( stdout );
               letter = ( char )getchar();
               while( letter == '\n' )
               {
                    letter = ( char )getchar();
               }
               if ( letter == 'Y' )
               {
                    letter = 'y';
               }
               else if ( letter == 'N' )
               {
                    letter = 'n';
               }
               if ( letter != 'y' && letter != 'n' )
               {
                    printf( "\n\
Sorry, that is not a valid answer.  Please answer with y or n.\n" );
                    ret = 0;
               }
               else
               {
                    ret = 1;
               }
          }    while( ret == 0 );
     }
     else /* prevent_file == 1 */
     {
          letter = 'n';
     }

     if ( letter == 'y' )
     {
          count = 0;
          do
          {
               ret = 0;

               /* Just using this variable because */
               /* it isn't being used yet.         */

               if ( count == 0 )
               {
                    printf( "\nWhat file name would you like to use? > " );
               }
               count = 0;
               fflush( stdout );
               if ( fgets( name, BUFFER_SIZE, stdin ) == NULL )
               {
                    printf( "\n\
Sorry, that is not an acceptable file name.\n\n" );
               }
               else
               {
                    ret = strlen( name );

                    /* Will it ever be less than 1 at this point? */

                    if ( ret > 0 )
                    {    /* Replace the newline character with null. */
                         ret--;
                         if ( name[ ret ] == '\n' )
                         {
                              name[ ret ] = 0;
                         }
                    }
                    if ( ret == 0 )
                    {
                         count = 1;  /* Silently loop around again. */
                    }
                    ret = 0;
                    if ( count == 0 )
                    {
                         fp = fopen( name, "a" );
                         if ( fp == NULL )
                         {
                              printf( "\n\
Sorry, could not open file: \"%s\"\n", name );
                              printf( "Reason: %s.\n", strerror( errno ) );
                              printf( "\
Please choose another file name.\n" );
                         }
                         else
                         {
                              ret = stat( name, &status );
                              if ( ret != 0 )
                              {
                                   printf( "\n\
Sorry, there was a problem gathering information about the file: \"%s\"\n",
                                           name );
                                   printf( "Reason: %s.\n",
                                           strerror( errno ) );
                                   printf( "\
Please choose another file name.\n" );
                                   fclose( fp );
                                   fp = NULL;
                                   ret = 0;
                              }
                              else
                              {

#ifdef DEBUG

                                   printf( "\nFile size: %d\n",
                                           status.st_size );

#endif

                                   if ( status.st_size > 0 )
                                   {
                                        /* Add a blank line between runs. */

                                        fprintf( fp, "\n" );
                                        ret = check_error( fp, name, 0 );
                                        if ( ret == 1 )
                                        {
                                             use_file = -1;
                                        }
                                        else
                                        {
                                             use_file = 1;
                                        }
                                   }
                                   else  /* File is empty. */
                                   {
                                        use_file = 1;
                                   }
                                   ret = 1;  /* Exit do-while loop. */

#ifdef RUN_AS_APP
#ifdef USE_DATA

                                   /* Set the permission bits. */

                                   if ( status.st_mode != 0100666 )
                                   {
                                        errno = 0;
                                        if ( chmod( name, 0100666 ) != 0 )
                                        {
                                             fprintf( stderr, "\
Failed to correctly set the permission bits of the output file.\n" );
                                             fprintf( stderr, "\
Error: %s.\n\n", strerror( errno ) );
                                             fclose( fp );
                                             fp = NULL;
                                             use_file = -1;
                                        }
                                   }

#endif
#endif

                              }    /* stat() */
                         }    /* fopen() */
                    }    /* if ( count == 0 ) */
               }   /* fgets() */
          }    while( ret == 0 );
     }
     else  /* letter == 'n' */
     {
          fp = NULL;
          use_file = 0;
     }

#ifdef DEBUG

#ifdef NEGATIVE_IS_NOT_PRIME

     if ( use_file == 1 )
     {
          ret = fprintf( fp, "Strict sign checking is enabled.\n" );
          if ( ret < 1 )
          {
               fprintf( stderr, "Failed to write to the output file.\n" );
               fprintf( stderr, "Reason: %s.\n\n", strerror( errno ) );
               use_file = -1;
               fclose( fp );
               fp = NULL;
          }
          else
          {
               ret = check_error( fp, name, 0 );
               if ( ret == 1 )
               {
                    use_file = -1;
               }
          }
     }

#else

     if ( use_file == 1 )
     {
          ret = fprintf( fp, "Strict sign checking is not enabled.\n" );
          if ( ret < 1 )
          {
               fprintf( stderr, "Failed to write to the output file.\n" );
               fprintf( stderr, "Reason: %s.\n\n", strerror( errno ) );
               use_file = -1;
               fclose( fp );
               fp = NULL;
          }
          else
          {
               ret = check_error( fp, name, 0 );
               if ( ret == 1 )
               {
                    use_file = -1;
               }
          }
     }

#endif

#ifdef LIST_ONE_AS_PRIME

     if ( use_file == 1 )
     {
          ret = fprintf( fp,
                         "Listing 1 with any prime numbers found.\n\n" );
          if ( ret < 1 )
          {
               fprintf( stderr, "Failed to write to the output file.\n" );
               fprintf( stderr, "Reason: %s.\n\n", strerror( errno ) );
               use_file = -1;
               fclose( fp );
               fp = NULL;
          }
          else
          {
               ret = check_error( fp, name, 0 );
               if ( ret == 1 )
               {
                    use_file = -1;
               }
          }
     }

#else

     if ( use_file == 1 )
     {
          ret = fprintf( fp, "\
Not listing 1 with any prime numbers found.\n\n" );
          if ( ret < 1 )
          {
               fprintf( stderr, "Failed to write to the output file.\n" );
               fprintf( stderr, "Reason: %s.\n\n", strerror( errno ) );
               use_file = -1;
               fclose( fp );
               fp = NULL;
          }
          else
          {
               ret = check_error( fp, name, 0 );
               if ( ret == 1 )
               {
                    use_file = -1;
               }
          }
     }

#endif

#endif  /* #ifdef DEBUG */

     if ( use_file == 1 )
     {
          ret = fprintf( fp, "\
Finding prime numbers between %d and %d inclusive.\n\n",
                        start, stop );
          if ( ret < 1 )
          {
               fprintf( stderr, "Failed to write to the output file.\n" );
               fprintf( stderr, "Reason: %s.\n\n", strerror( errno ) );
               use_file = -1;
               fclose( fp );
               fp = NULL;
          }
          else
          {
               ret = check_error( fp, name, 0 );
               if ( ret == 1 )
               {
                    use_file = -1;
               }
          }
     }

#endif  /* #ifdef COPY_OUTPUT_TO_FILE */

/*

     We need to ask the user if they want the display of the prime numbers
     themselves supressed so only the total number of prime numbers found
     will be displayed.  We also need to give the user the flexibility of
     sending the full output to a file while doing this.

*/

#ifdef COPY_OUTPUT_TO_FILE

     printf( "\n\
You may choose to suppress the display\n\
of the prime numbers so only the total\n\
number of prime numbers found will be\n\
displayed.  This can speed things up.\n" );

     if ( use_file == 1 )
     {
          do
          {
               printf( "\nPlease choose your display preference:\n\n" );
               printf( "\
1) Show the prime numbers on the screen\n\
   and in the file.\n" );
               printf( "\
2) Send the prime numbers to the file,\n\
   but don't display them on the screen.\n" );
               printf( "\
3) Don't show the prime numbers on the\n\
   screen or in the file.\n\n" );
               printf( "> " );
               fflush( stdout );
               ret = scanf( "%d", &show_numbers );
               if ( ret != 1 )
               {
                    printf( "\n\
Sorry, that is not a valid option.  Please choose either 1, 2, or 3.\n" );
                    ret = 0;
               }
               else if ( show_numbers != 1 && show_numbers != 2 &&
                             show_numbers != 3 )
               {
                    printf( "\n\
Sorry, that is not a valid option.  Please choose either 1, 2, or 3.\n" );
                    ret = 0;
               }
          }    while( ret == 0 );
     }
     else  /* use_file == 0 or -1 */
     {
          do
          {
               printf( "\nPlease choose your display preference:\n\n" );
               printf( "1) Show the prime numbers on the screen.\n" );
               printf( "\
2) Don't display the prime numbers on the screen.\n\n" );
               printf( "> " );
               fflush( stdout );
               ret = scanf( "%d", &show_numbers );
               if ( ret != 1 )
               {
                    printf( "\n\
Sorry, that is not a valid option.  Please choose either 1 or 2.\n" );
                    ret = 0;
               }
               else if ( show_numbers != 1 && show_numbers != 2 )
               {
                    printf( "\n\
Sorry, that is not a valid option.  Please choose either 1 or 2.\n" );
                    ret = 0;
               }
               else
               {
                    if ( show_numbers == 2 )
                    {    /* This will be important later. */
                         show_numbers = 3;
                    }
               }
          }    while( ret == 0 );
     }   /* if ( use_file == 1 or 0 ) */

#else  /* COPY_OUTPUT_TO_FILE is not #defined. */

     printf( "\n\
You may choose to suppress the display\n\
of the prime numbers so only the total\n\
number of prime numbers found will be\n\
displayed.  This can speed things up.\n" );

     do
     {
          printf( "\nPlease choose your display preference:\n\n" );
          printf( "1) Show the prime numbers on the screen.\n" );
          printf( "2) Don't display the prime numbers on the screen.\n\n" );
          printf( "> " );
          fflush( stdout );
          ret = scanf( "%d", &show_numbers );
          if ( ret != 1 )
          {
               printf( "\n\
Sorry, that is not a valid option.  Please choose either 1 or 2.\n" );
               ret = 0;
          }
          else if ( show_numbers != 1 && show_numbers != 2 )
          {
               printf( "\n\
Sorry, that is not a valid option.  Please choose either 1 or 2.\n" );
               ret = 0;
          }
          else
          {
               if ( show_numbers == 2 )
               {
                    show_numbers = 3;  /* This will be important later. */
               }
          }
     }    while( ret == 0 );

#endif

/*

     Now we need to find out if the user wants the total
     number of prime numbers found to be displayed.

*/

#ifdef COPY_OUTPUT_TO_FILE

     if ( use_file == 1 )
     {
          do
          {
               printf( "\nPlease choose your display preference:\n\n" );
               printf( "\
1) Show the total number of prime numbers\n\
   found on the screen and in the file.\n" );
               printf( "\
2) Show the total number of prime numbers found\n\
   in the file, but don't display them on the screen.\n" );
               printf( "\
3) Don't show the total number of prime\n\
   numbers found on the screen or in the file.\n\n" );
               printf( "> " );
               fflush( stdout );
               ret = scanf( "%d", &show_total );
               if ( ret != 1 )
               {
                    printf( "\n\
Sorry, that is not a valid option.  Please choose either 1, 2, or 3.\n" );
                    ret = 0;
               }
               else if ( show_total != 1 && show_total != 2 &&
                             show_total != 3 )
               {
                    printf( "\n\
Sorry, that is not a valid option.  Please choose either 1, 2, or 3.\n" );
                    ret = 0;
               }
          }    while( ret == 0 );
     }
     else  /* use_file == 0 or -1 */
     {
          do
          {
               printf( "\nPlease choose your display preference:\n\n" );
               printf( "\
1) Show the total number of prime numbers found on the screen.\n" );
               printf( "\
2) Don't show the total number of prime numbers found on the \
screen.\n\n" );
               printf( "> " );
               fflush( stdout );
               ret = scanf( "%d", &show_total );
               if ( ret != 1 )
               {
                    printf( "\n\
Sorry, that is not a valid option.  Please choose either 1 or 2.\n" );
                    ret = 0;
               }
               else if ( show_total != 1 && show_total != 2 )
               {
                    printf( "\n\
Sorry, that is not a valid option.  Please choose either 1 or 2.\n" );
                    ret = 0;
               }
               else
               {
                    if ( show_total == 2 )
                    {    /* This will be important later. */
                         show_total = 3;
                    }
               }
          }    while( ret == 0 );
     }   /* if ( use_file == 1 or 0 ) */

#else  /* COPY_OUTPUT_TO_FILE is not #defined. */

     do
     {
          printf( "\nPlease choose your display preference:\n\n" );
          printf( "\
1) Show the total number of prime numbers found on the screen.\n" );
          printf( "\
2) Don't show the total number of prime numbers found on the screen.\n\n" );
          printf( "> " );
          fflush( stdout );
          ret = scanf( "%d", &show_total );
          if ( ret != 1 )
          {
               printf( "\n\
Sorry, that is not a valid option.  Please choose either 1 or 2.\n" );
               ret = 0;
          }
          else if ( show_total != 1 && show_total != 2 )
          {
               printf( "\n\
Sorry, that is not a valid option.  Please choose either 1 or 2.\n" );
               ret = 0;
          }
          else
          {
               if ( show_total == 2 )
               {
                    show_total = 3;  /* This will be important later. */
               }
          }
     }    while( ret == 0 );

#endif

/*

     At this point we have our starting and stopping points.
     Now we need to find all of the prime numbers between those
     two inclusive points and display them.

*/

     printf( "\n" );
     total = 0;

#ifdef COPY_OUTPUT_TO_FILE

     if ( use_file == 1 )
     {
          for( count = start; count <= stop; count++ )
          {

#ifdef DEBUG

               ret = is_prime( count );
               if ( show_numbers == 1 )
               {
                    printf( "is_prime( %d ) = %d\n", count, ret );
               }
               if ( ret == 1 )
               {
                    total++;
               }
               if ( use_file == 1 && show_numbers != 3 )
               {
                    if ( fprintf( fp, "is_prime( %d ) = %d\n",
                                  count, ret ) < 1 )
                    {
                         fprintf( stderr, "\
Failed to write to the output file.\n" );
                         fprintf( stderr, "\
Reason: %s.\n\n", strerror( errno ) );
                         use_file = -1;
                         fclose( fp );
                         fp = NULL;

                         if ( show_numbers != 1 && show_total != 1 )
                         {
                              count = stop;
                         }
                    }
                    else
                    {
                         ret = check_error( fp, name, 0 );
                         if ( ret == 1 )
                         {
                              use_file = -1;

                              /* It's pointless to continue */
                              /* if we show nothing.        */

                              if ( show_numbers != 1 && show_total != 1 )
                              {
                                   count = stop;
                              }
                         }
                    }
               }

#else

               if ( is_prime( count ) )
               {
                    if ( one_per_line == 1 )
                    {
                         if ( show_numbers == 1 )
                         {
                              printf( "%d\n", count );
                         }

                         /* use_file is changed to -1 if */
                         /* we have problems with fp.    */

                         if ( use_file == 1 && show_numbers != 3 )
                         {
                              if ( fprintf( fp, "%d\n", count ) < 1 )
                              {
                                   fprintf( stderr, "\
Failed to write to the output file.\n" );
                                   fprintf( stderr, "\
Reason: %s.\n\n", strerror( errno ) );
                                   use_file = -1;
                                   fclose( fp );
                                   fp = NULL;

                                   if ( show_numbers != 1 &&
                                        show_total != 1 )
                                   {
                                        count = stop;
                                   }
                              }
                              else
                              {
                                   ret = check_error( fp, name, 0 );
                                   if ( ret == 1 )
                                   {
                                        use_file = -1;

                                        /* It's pointless to continue */
                                        /* if we show nothing.        */

                                        if ( show_numbers != 1 &&
                                             show_total != 1 )
                                        {
                                             count = stop;
                                        }
                                   }
                              }
                         }
                    }
                    else  /* Let's hope the user   */
                    {     /* has autowrap support. */
                         if ( show_numbers == 1 )
                         {
                              printf( "%d ", count );
                              fflush( stdout );
                         }
                         if ( use_file == 1 && show_numbers != 3 )
                         {
                              if ( fprintf( fp, "%d ", count ) < 1 )
                              {
                                   fprintf( stderr, "\
Failed to write to the output file.\n" );
                                   fprintf( stderr, "\
Reason: %s.\n\n", strerror( errno ) );
                                   use_file = -1;
                                   fclose( fp );
                                   fp = NULL;

                                   if ( show_numbers != 1 &&
                                        show_total != 1 )
                                   {
                                        count = stop;
                                   }
                              }
                              else
                              {
                                   ret = check_error( fp, name, 1 );
                                   if ( ret == 1 )
                                   {
                                        use_file = -1;

                                        /* It's pointless to continue */
                                        /* if we show nothing.        */

                                        if ( show_numbers != 1 &&
                                             show_total != 1 )
                                        {
                                             count = stop;
                                        }
                                   }
                              }
                         }
                    }
                    total++;
               }    /* if ( is_prime( count ) ) */

#endif

          }  /* for() */
     }
     else if ( use_file == 0 )
     {
          for( count = start; count <= stop; count++ )
          {

#ifdef DEBUG

               ret = is_prime( count );
               if ( show_numbers == 1 )
               {
                    printf( "is_prime( %d ) = %d\n", count, ret );
               }
               if ( ret == 1 )
               {
                    total++;
               }

#else

               if ( is_prime( count ) )
               {
                    if ( show_numbers == 1 )
                    {
                         if ( one_per_line == 1 )
                         {
                              printf( "%d\n", count );
                         }
                         else  /* Let's hope the user   */
                         {     /* has autowrap support. */
                              printf( "%d ", count );
                              fflush( stdout );
                         }
                    }
                    total++;
               }

#endif

          }  /* for() */
     }  /*  if ( use_file == 1 or 0 ) */
     else  /* use_file was -1 before we started checking. */
     {
          total = -1;
     }

#else  /* COPY_OUTPUT_TO_FILE is not #defined. */

     for( count = start; count <= stop; count++ )
     {

#ifdef DEBUG

          ret = is_prime( count );
          if ( show_numbers == 1 )
          {
               printf( "is_prime( %d ) = %d\n", count, ret );
          }
          if ( ret == 1 )
          {
               total++;
          }

#else

          if ( is_prime( count ) )
          {
               if ( show_numbers == 1 )
               {
                    if ( one_per_line == 1 )
                    {
                         printf( "%d\n", count );
                    }
                    else  /*  Let's hope the user has autowrap support. */
                    {
                         printf( "%d ", count );
                         fflush( stdout );
                    }
               }
               total++;
          }

#endif

     }  /* for() */

#endif  /* #ifdef COPY_OUTPUT_TO_FILE */

     if ( total == ( -1 ) )
     {    /* We had problems with the output file before testing. */
          if ( show_total == 1 )
          {
               printf( "You did not test any numbers.\n" );
          }
     }
     else if ( total == 0 )
     {
          if ( show_total == 1 )
          {
               ret = stop - start + 1;
               if ( ret == 1 )
               {
                    printf( "\
You only tested 1 number and it was not a prime number.\n" );
               }
               else if ( ret == 2 )
               {
                    printf( "\
You tested 2 different numbers and neither of them were prime numbers.\n" );
               }
               else
               {
                     printf( "\
You tested %d different numbers and none of them were prime numbers.\n",
                             ret );
               }
          }

#ifdef COPY_OUTPUT_TO_FILE

          if ( use_file == 1 && show_total != 3 )
          {
               ret = stop - start + 1;
               if ( ret == 1 )
               {
                    if ( fprintf( fp, "\
You only tested 1 number and it was not a prime number.\n" ) < 1 )
                    {
                         fprintf( stderr, "\
Failed to write to the output file.\n" );
                         fprintf( stderr, "\
Reason: %s.\n\n", strerror( errno ) );
                         use_file = -1;
                         fclose( fp );
                         fp = NULL;
                    }
               }
               else if ( ret == 2 )
               {
                    if ( fprintf( fp, "\
You tested 2 different numbers and neither of them were prime numbers.\
\n" ) < 1 )
                    {
                         fprintf( stderr, "\
Failed to write to the output file.\n" );
                         fprintf( stderr, "\
Reason: %s.\n\n", strerror( errno ) );
                         use_file = -1;
                         fclose( fp );
                         fp = NULL;
                    }
               }
               else
               {
                    if ( fprintf( fp, "\
You tested %d different numbers and none of them were prime numbers.\n",
                                  ret ) < 1 )
                    {
                         fprintf( stderr, "\
Failed to write to the output file.\n" );
                         fprintf( stderr, "\
Reason: %s.\n\n", strerror( errno ) );
                         use_file = -1;
                         fclose( fp );
                         fp = NULL;
                    }
               }
               if ( use_file == 1 )
               {
                    ret = check_error( fp, name, 0 );
                    if ( ret == 1 )
                    {
                         use_file = -1;
                    }
               }
          }

#endif

     }
     else if ( total == 1 )
     {

#ifdef DEBUG

          if ( show_total == 1 )
          {
               ret = stop - start + 1;
               if ( ret == 1 )
               {
                    if ( show_numbers == 1 )
                    {
                         printf( "\n" );
                    }
                    printf( "\
You only tested 1 number and it was a prime number.\n" );
               }
               else
               {
                    if ( show_numbers == 1 )
                    {
                         printf( "\n" );
                    }
                    printf( "\
You tested %d different numbers and only 1 of them was a prime number.\n",
                            ret );
               }
          }

#else

          if ( show_total == 1 )
          {
               ret = stop - start + 1;
               if ( one_per_line == 1 )
               {
                    if ( ret == 1 )
                    {
                         if ( show_numbers == 1 )
                         {
                              printf( "\n" );
                         }
                         printf( "\
You only tested 1 number and it was a prime number.\n" );
                    }
                    else
                    {
                         if ( show_numbers == 1 )
                         {
                              printf( "\n" );
                         }
                         printf( "\
You tested %d different numbers and only 1 of them was a prime number.\n",
                                 ret );
                    }
               }
               else
               {
                    if ( ret == 1 )
                    {
                         if ( show_numbers == 1 )
                         {
                              printf( "\n\n" );
                         }
                         printf( "\
You only tested 1 number and it was a prime number.\n" );
                    }
                    else
                    {
                         if ( show_numbers == 1 )
                         {
                              printf( "\n\n" );
                         }
                         printf( "\
You tested %d different numbers and only 1 of them was a prime number.\n",
                                 ret );
                    }
               }
          }

#endif

#ifdef COPY_OUTPUT_TO_FILE

          if ( use_file == 1 && show_total != 3 )
          {

#ifdef DEBUG

               ret = stop - start + 1;
               if ( ret == 1 )
               {
                    if ( fprintf( fp, "\n\
You only tested 1 number and it was a prime number.\n" ) < 1 )
                    {
                         fprintf( stderr, "\
Failed to write to the output file.\n" );
                         fprintf( stderr, "\
Reason: %s.\n\n", strerror( errno ) );
                         use_file = -1;
                         fclose( fp );
                         fp = NULL;
                    }
               }
               else
               {
                    if ( fprintf( fp, "\n\
You tested %d different numbers and only one of them was a prime number.\n",
                                  ret ) < 1 )
                    {
                         fprintf( stderr, "\
Failed to write to the output file.\n" );
                         fprintf( stderr, "\
Reason: %s.\n\n", strerror( errno ) );
                         use_file = -1;
                         fclose( fp );
                         fp = NULL;
                    }
               }

#else

               ret = stop - start + 1;
               if ( one_per_line == 1 )
               {
                    if ( ret == 1 )
                    {
                         if ( fprintf( fp, "\n\
You only tested 1 number and it was a prime number.\n" ) < 1 )
                         {
                              fprintf( stderr, "\
Failed to write to the output file.\n" );
                              fprintf( stderr, "\
Reason: %s.\n\n", strerror( errno ) );
                              use_file = -1;
                              fclose( fp );
                              fp = NULL;
                         }
                    }
                    else
                    {
                         if ( fprintf( fp, "\n\
You tested %d different numbers and only 1 of them was a prime number.\n",
                                       ret ) < 1 )
                         {
                              fprintf( stderr, "\
Failed to write to the output file.\n" );
                              fprintf( stderr, "\
Reason: %s.\n\n", strerror( errno ) );
                              use_file = -1;
                              fclose( fp );
                              fp = NULL;
                         }
                    }
               }
               else
               {
                    if ( ret == 1 )
                    {
                         if ( fprintf( fp, "\n\n\
You only tested 1 number and it was a prime number.\n" ) < 1 )
                         {
                              fprintf( stderr, "\
Failed to write to the output file.\n" );
                              fprintf( stderr, "\
Reason: %s.\n\n", strerror( errno ) );
                              use_file = -1;
                              fclose( fp );
                              fp = NULL;
                         }
                    }
                    else
                    {
                         if ( fprintf( fp, "\n\n\
You tested %d different numbers and only 1 of them was a prime number.\n",
                                       ret ) < 1 )
                         {
                              fprintf( stderr, "\
Failed to write to the output file.\n" );
                              fprintf( stderr, "\
Reason: %s.\n\n", strerror( errno ) );
                              use_file = -1;
                              fclose( fp );
                              fp = NULL;
                         }
                    }
               }

#endif

               if ( use_file == 1 )
               {
                    ret = check_error( fp, name, 0 );
                    if ( ret == 1 )
                    {
                         use_file = -1;
                    }
               }
          }

#endif  /* #ifdef COPY_OUTPUT_TO_FILE */

     }
     else  /* total > 1 */
     {

#ifdef DEBUG

          if ( show_total == 1 )
          {
               ret = stop - start + 1;
               if ( ret == 2 )
               {
                    if ( show_numbers == 1 )
                    {
                         printf( "\n" );
                    }
                    printf( "\
You tested 2 different numbers and both of them were prime numbers.\n" );
               }
               else if ( ret == total )  /* 1, 2, and 3 */
               {
                    if ( show_numbers == 1 )
                    {
                         printf( "\n" );
                    }
                    printf( "\
You tested %d different numbers and all of them were prime numbers.\n",
                            ret );
               }
               else
               {
                    if ( show_numbers == 1 )
                    {
                         printf( "\n" );
                    }
                    printf( "\
You tested %d different numbers and %d of them were prime numbers.\n",
                            ret, total );
               }
          }

#else

          if ( show_total == 1 )
          {
               ret = stop - start + 1;
               if ( one_per_line == 1 )
               {
                    if ( ret == 2 )
                    {
                         if ( show_numbers == 1 )
                         {
                              printf( "\n" );
                         }
                         printf( "\
You tested 2 different numbers and both of them were prime numbers.\n" );
                    }
                    else if ( ret == total )
                    {
                         if ( show_numbers == 1 )
                         {
                              printf( "\n" );
                         }
                         printf( "\
You tested %d different numbers and all of them were prime numbers.\n",
                                 ret );
                    }
                    else
                    {
                         if ( show_numbers == 1 )
                         {
                              printf( "\n" );
                         }
                         printf( "\
You tested %d different numbers and %d of them were prime numbers.\n",
                                 ret, total );
                    }
               }
               else
               {
                    if ( ret == 2 )
                    {
                         if ( show_numbers == 1 )
                         {
                              printf( "\n\n" );
                         }
                         printf( "\
You tested 2 different numbers and both of them were prime numbers.\n" );
                    }
                    else if ( ret == total )
                    {
                         if ( show_numbers == 1 )
                         {
                              printf( "\n\n" );
                         }
                         printf( "\
You tested %d different numbers and all of them were prime numbers.\n",
                                 ret );
                    }
                    else
                    {
                         if ( show_numbers == 1 )
                         {
                              printf( "\n\n" );
                         }
                         printf( "\
You tested %d different numbers and %d of them were prime numbers.\n",
                                 ret, total );
                    }
               }
          }

#endif

#ifdef COPY_OUTPUT_TO_FILE

          if ( use_file == 1 && show_total != 3 )
          {
               ret = stop - start + 1;

#ifdef DEBUG

               if ( ret == 2 )
               {
                    if ( fprintf( fp, "\n\
You tested 2 different numbers and both of them were prime numbers.\
\n" ) < 1 )
                    {
                         fprintf( stderr, "\
Failed to write to the output file.\n" );
                         fprintf( stderr, "\
Reason: %s.\n\n", strerror( errno ) );
                         use_file = -1;
                         fclose( fp );
                         fp = NULL;
                    }
               }
               else if ( ret == total )  /* 1, 2, and 3 */
               {
                    if ( fprintf( fp, "\n\
You tested %d different numbers and all of them were prime numbers.\n",
                                  ret ) < 1 )
                    {
                         fprintf( stderr, "\
Failed to write to the output file.\n" );
                         fprintf( stderr, "\
Reason: %s.\n\n", strerror( errno ) );
                         use_file = -1;
                         fclose( fp );
                         fp = NULL;
                    }
               }
               else
               {
                    if ( fprintf( fp, "\n\
You tested %d different numbers and %d of them were prime numbers.\n",
                                  ret, total ) < 1 )
                    {
                         fprintf( stderr, "\
Failed to write to the output file.\n" );
                         fprintf( stderr, "\
Reason: %s.\n\n", strerror( errno ) );
                         use_file = -1;
                         fclose( fp );
                         fp = NULL;
                    }
               }

#else

               if ( one_per_line == 1 )
               {
                    if ( ret == 2 )
                    {
                         if ( fprintf( fp, "\n\
You tested 2 different numbers and both of them were prime numbers.\
\n" ) < 1 )
                         {
                              fprintf( stderr, "\
Failed to write to the output file.\n" );
                              fprintf( stderr, "\
Reason: %s.\n\n", strerror( errno ) );
                              use_file = -1;
                              fclose( fp );
                              fp = NULL;
                         }
                    }
                    else if ( ret == total )  /* 1, 2, and 3 */
                    {
                         if ( fprintf( fp, "\n\
You tested %d different numbers and all of them were prime numbers.\n",
                                       ret ) < 1 )
                         {
                              fprintf( stderr, "\
Failed to write to the output file.\n" );
                              fprintf( stderr, "\
Reason: %s.\n\n", strerror( errno ) );
                              use_file = -1;
                              fclose( fp );
                              fp = NULL;
                         }
                    }
                    else
                    {
                         if ( fprintf( fp, "\n\
You tested %d different numbers and %d of them were prime numbers.\n",
                                       ret, total ) < 1 )
                         {
                              fprintf( stderr, "\
Failed to write to the output file.\n" );
                              fprintf( stderr, "\
Reason: %s.\n\n", strerror( errno ) );
                              use_file = -1;
                              fclose( fp );
                              fp = NULL;
                         }
                    }
               }
               else
               {
                    if ( ret == 2 )
                    {
                         if ( fprintf( fp, "\n\n\
You tested 2 different numbers and both of them were prime numbers.\
\n" ) < 1 )
                         {
                              fprintf( stderr, "\
Failed to write to the output file.\n" );
                              fprintf( stderr, "\
Reason: %s.\n\n", strerror( errno ) );
                              use_file = -1;
                              fclose( fp );
                              fp = NULL;
                         }
                    }
                    else if ( ret == total )  /* 1, 2, and 3 */
                    {
                         if ( fprintf( fp, "\n\n\
You tested %d different numbers and all of them were prime numbers.\n",
                                       ret ) < 1 )
                         {
                              fprintf( stderr, "\
Failed to write to the output file.\n" );
                              fprintf( stderr, "\
Reason: %s.\n\n", strerror( errno ) );
                              use_file = -1;
                              fclose( fp );
                              fp = NULL;
                         }
                    }
                    else
                    {
                         if ( fprintf( fp, "\n\n\
You tested %d different numbers and %d of them were prime numbers.\n",
                                       ret, total ) < 1 )
                         {
                              fprintf( stderr, "\
Failed to write to the output file.\n" );
                              fprintf( stderr, "\
Reason: %s.\n\n", strerror( errno ) );
                              use_file = -1;
                              fclose( fp );
                              fp = NULL;
                         }
                    }
               }

#endif

               if ( use_file == 1 )
               {
                    ret = check_error( fp, name, 0 );
                    if ( ret == 1 )
                    {
                         use_file = -1;
                    }
               }
          }    /* if ( use_file == 1 ) */

#endif  /* #ifdef COPY_OUTPUT_TO_FILE */

     }  /* if() number of prime numbers found */

#ifdef COPY_OUTPUT_TO_FILE

     if ( use_file == 1 )
     {
          fclose( fp );
     }
     else if ( use_file == ( -1 ) )
     {
          if ( show_numbers != 1 && show_total != 1 )
          {
               printf( "Exiting.\n\n" );
          }
          return 1;
     }

#endif

/*

     Let the user know when the program is finished running
     when they aren't displaying anything on the screen.

*/

     if ( show_numbers != 1 && show_total != 1 )
     {
          printf( "Program finished.  Exiting.\n\n" );
     }
     else
     {
          printf( "\n" );
     }

     return 0;
}

/*

     This is the work horse function of this program.
     This is where we find out if the number we are
     testing is a prime number.  A prime number is a
     number that only has the factors of 1 and itself.

*/

int is_prime( int num )
{
     int limit;
     register int count;

     if ( num == 0 )
     {
          return 0;  /* Zero is not a prime number. */
     }

/*

     This code may be copied and used in someone
     else's program and if that be the case then
     we can't be sure about the input passed to
     this function.

*/

     if ( num < 0 )
     {

#ifdef NEGATIVE_IS_NOT_PRIME

          return 0;  /* Strict */

#endif

          num *= ( -1 );  /* Just flip it and use it anyway. */
     }

/*

     Some may want 1 listed with prime numbers.
     If they do they can #define LIST_ONE_AS_PRIME
     at the top of the program with the other definitions.

*/

#ifdef LIST_ONE_AS_PRIME

     if ( num == 1 )
     {
          return 1;
     }

#else

     if ( num == 1 )
     {
          return 0;
     }

#endif

/*

     Take care of an obvious case.

*/

     if ( num == 2 )
     {
          return 1;  /* 2 is a prime number. */
     }

/*

     Any even number greater than 2 is not a prime number because
     it is evenly divisible by 2 so we shouldn't waste our time with it.

*/

     if ( num % 2 == 0 )
     {
          return 0;
     }

/*

     At this point we know that the number is an odd number greater
     than 2.  The number cannot have any factors, other than itself,
     that are greater than the square root of the number that we are
     testing so it is pointless to search for them.  We will increment
     count by two steps so we don't search for even numbered factors.
     If the number we are testing is 3 then the for() loop won't even
     be used.

*/

     limit = ( int )sqrt( num );
     for ( count = 3; count <= limit; count += 2 )
     {
          if ( num % count == 0 )
          {
               return 0;  /* We found a factor so num */
          }               /* is not a prime number.   */
     }

/*

     We didn't find any factors so num is a prime number.

*/

     return 1;
}

#ifdef COPY_OUTPUT_TO_FILE

/*

     This function checks to see if we've had any
     problems writing to the output file and tells
     the user about it and closes the file if we have.

     Returns 1 if the file stream has a problem, otherwise 0.

*/

int check_error( FILE *fp, char *name, int add_line )
{
     if ( fp == NULL )
     {
          return 1;  /* The specified file pointer is not valid. */
     }
     name[ ( BUFFER_SIZE - 1 ) ] = 0;  /* Make sure. */
     if ( add_line != 0 && add_line != 1 )
     {
          add_line = 0;
     }
     if ( ferror( fp ) )
     {
          if ( add_line == 1 )
          {
               fprintf( stderr, "\n" );
          }
          fprintf( stderr, "\n\
Sorry, there was a problem accessing the file: \"%s\"\n", name );
          fprintf( stderr, "Reason: %s.\n", strerror( errno ) );
          fprintf( stderr, "\
Program output will no longer be copied to it.\n" );
          fclose( fp );
          return 1;
     }
     fflush( fp );
     if ( ferror( fp ) )
     {
          if ( add_line == 1 )
          {
               fprintf( stderr, "\n" );
          }
          fprintf( stderr, "\n\
Sorry, there was a problem accessing the file: \"%s\"\n", name );
          fprintf( stderr, "Reason: %s.\n", strerror( errno ) );
          fprintf( stderr, "Program output will no longer be copied to it.\
\n" );
          fclose( fp );
          return 1;
     }
     return 0;
}

#ifdef RUN_AS_APP

#ifdef USE_DATA

/*

     This function sets up a directory to allow writing the program's
     output to a text file if it running as a stand alone app.  This
     will also allow a command line access app to access the output file.

*/

int setup_directory( char *program_name )
{
     char directory[ 1025 ];
     int count, pos, size;

     if ( program_name == NULL )
     {
          fprintf( stderr,
                   "\nNull pointer passed to setup_directory().\n" );
          return 1;  /* Error. */
     }
     if ( program_name[ 0 ] == 0 )
     {
          fprintf( stderr,
                   "\nEmpty string passed to setup_directory().\n" );
          return 1;  /* Error. */
     }

     size = strlen( program_name );
     pos = size - 1;
     while( pos >= 0 && program_name[ pos ] != '/' )
     {
          pos--;
     }
     if ( pos < 0 )  /* Apps always use an absolute path name. */
     {
          fprintf( stderr, "\n\
Failed to determine the app's directory.\n" );
          return 1;
     }
     for( count = 0; count < pos; count++ )
     {
          directory[ count ] = program_name[ count ];
     }
     directory[ count ] = 0;
     errno = 0;
     if ( chmod( directory, 0755 ) != 0 )
     {
          count = errno;
          fprintf( stderr, "\n\
Failed to correctly set the permission bits for the parent directory.\n" );
          fprintf( stderr, "Error: %s.\n", strerror( count ) );
          return 1;
     }
     directory[ count++ ] = '/';
     directory[ count++ ] = 'f';
     directory[ count++ ] = 'i';
     directory[ count++ ] = 'l';
     directory[ count++ ] = 'e';
     directory[ count++ ] = 's';
     directory[ count++ ] = 0;

     /* There's no real harm in trying if it already exists. */

     errno = 0;
     if ( mkdir( directory, 0777 ) )
     {
          count = errno;
          if ( errno != EEXIST )
          {
               fprintf( stderr, "\n\
Failed to create a directory for the output file.\n" );
               fprintf( stderr, "Error: %s.\n", strerror( count ) );
               return 1;
          }
     }
     else
     {
          errno = 0;
          if ( chmod( directory, 0777 ) != 0 )
          {
               count = errno;
               fprintf( stderr, "\n\
Failed to correctly set the permission bits for the new directory.\n" );
               fprintf( stderr, "Error: %s.\n", strerror( count ) );
               return 1;
          }
     }
     errno = 0;
     if ( chdir( directory ) != 0 )
     {
          count = errno;
          fprintf( stderr, "\n\
Failed to change the current working directory to the new directory.\n" );
          fprintf( stderr, "Error: %s.\n", strerror( count ) );
          return 1;
     }

     printf( "\nUsing \"%s/\" to store the output file.\n", directory );

     return 0;
}

#endif

#endif

#endif

/* EOF prime.c */
