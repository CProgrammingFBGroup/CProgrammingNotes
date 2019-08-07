/*

     rocket.c

     Prints a rocket to the screen and launches it.

     Written as an example for the Programmer's
     Challenge #3 in the C Programming group.

     The #defined values are for use with my tablet.  Read through
     the program and edit it to customize it for your own system.

     Written by Matthew Campbell on Friday May 3, 2019.

*/

/*

     Programmer's Challenge #3 rules:

     Your assignment, should you choose to accept it, is to write
     a C program that prints a rocket on the screen so that it is
     centered at the bottom of the screen.  Your program must read
     the available number of lines and columns on the screen from
     the appropriate environment variables.  You may use printf(3)
     when printing an error message to the screen.  All other
     program output must print a character at a time using
     putchar(3).  You must print your rocket and its thrust when it
     blasts off using for() loops.  The for() loops must be nested
     when appropriate.  Your logic is your paint brush.  Let your
     rocket sit there for a little while before it blasts off so
     you have a chance to look at it first.  All characters passed
     to putchar(3) must be in the form of a decimal integer.  You
     must not use single quotes to get the compiler to do the work
     for you.  You will need to determine the correct numeric values
     yourself.  You must use printable seven bit ASCII characters
     throughout your program.  You must not use array variables to
     store or print your rocket or its thrust.

*/

/* nanosleep(2) needs this. */

#define _POSIX_C_SOURCE 199309L

/* Include the necessary header files. */

#include <time.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Define upper limits for an acceptable number of lines and columns. */

#define MAX_COLS 200
#define MAX_ROWS 150

/* Define how many lines tall our rocket is. */

#define LINES_TALL 22

/* Define how many columns wide our rocket is. */

#define COLUMNS_WIDE 16

/*

     Define NEED_LINES_AND_COLUMNS if your terminal emulator does
     not provide the environment variables LINES and COLUMNS.

*/

#define NEED_LINES_AND_COLUMNS

/* Define NEED_KEYBOARD_DELAY if you use an on-screen keyboard. */

#define NEED_KEYBOARD_DELAY

/* Prototype the clear_screen() function. */

void clear_screen( void );

/* Prototype the set_lines_and_columns() function. */

#ifdef NEED_LINES_AND_COLUMNS

int set_lines_and_columns( void );

#endif

/* Prototype the variable_delay() function. */

void variable_delay( void );

/* Prototype the print_rocket() function. */

void print_rocket( int use_blank_cols );

/* Prototype the apply_thrust() function. */

void apply_thrust( int use_blank_cols, int rows );

/* Define the main() function. */

int main( void )
{
     /* Define some local variables. */

     char *str_ptr;
     int cols, count, pos, rows, use_blank_cols, use_blank_rows;

     /*

          Unfortunately C4droid does not have values for
          LINES and COLUMNS so we'll need to set them.

     */

#ifdef NEED_LINES_AND_COLUMNS

     if ( set_lines_and_columns() == ( -1 ) )
     {
          printf( "\
Failed to set the environment variables LINES and COLUMNS.\n\n" );
          exit( EXIT_FAILURE );
     }

#endif

     /* Find the number of available lines on the screen. */

     str_ptr = getenv( "LINES" );

     /* Make sure we received something useful. */

     if ( str_ptr == NULL )
     {
          printf( "\
Failed to determine the number of lines on the screen.\n" );
          printf( "The environment variable LINES was not found.\n\n" );
          exit( EXIT_FAILURE );
     }

     if ( sscanf( str_ptr, "%d", &rows ) != 1 )
     {
          printf( "\
Failed to determine the number of lines on the screen.\n" );
          printf( "\
The environment variable LINES did not hold an integer value.\n\n" );
          exit( EXIT_FAILURE );
     }

     if ( rows < 1 || rows > MAX_ROWS )
     {
          printf( "\
The environment variable LINES did not hold an acceptable value.\n\n" );
          exit( EXIT_FAILURE );
     }

     /* Find out how many available columns are on the screen. */

     str_ptr = getenv( "COLUMNS" );

     /* Make sure we received something useful. */

     if ( str_ptr == NULL )
     {
          printf( "\
Failed to determine the number of columns on the screen.\n" );
          printf( "The environment variable COLUMNS was not found.\n\n" );
          exit( EXIT_FAILURE );
     }

     if ( sscanf( str_ptr, "%d", &cols ) != 1 )
     {
          printf( "\
Failed to determine the number of columns on the screen.\n" );
          printf( "\
The environment variable COLUMNS did not hold an integer value.\n\n" );
          exit( EXIT_FAILURE );
     }
     str_ptr = NULL;  /* No longer needed. */

     if ( cols < 1 || cols > MAX_COLS )
     {
          printf( "\
The environment variable COLUMNS did not hold an acceptable value.\n\n" );
          exit( EXIT_FAILURE );
     }

     /* Make sure the rocket will fit on the screen. */

     if ( cols < COLUMNS_WIDE )
     {
          printf( "The rocket is too wide to fit on the screen.\n\n" );
          exit( EXIT_FAILURE );
     }

     if ( rows < ( LINES_TALL + 1 ) )
     {
          printf( "The rocket is too tall to fit on the screen.\n\n" );
          exit( EXIT_FAILURE );
     }

     /*

          Find out how many blank lines and columns to print
          to center the rocket at the bottom of the screen.

     */

     use_blank_cols = ( cols - COLUMNS_WIDE ) / 2;

     /* Leave one blank line at the bottom of the screen. */

     use_blank_rows = rows - LINES_TALL - 1;

     /* Now we're ready to print our rocket. */

#ifdef NEED_KEYBOARD_DELAY

     sleep( 5 );

#endif

     /* First we will clear the screen. */

     clear_screen();

     /* Now print the correct number of blank lines. */

     for( count = 0; count < use_blank_rows; count++ )
     {
          putchar( 10 );  /* '\n' */
     }

     /* Print the rocket. */

     print_rocket( use_blank_cols );

     /* Give the user a chance to look at it. */

     sleep( 5 );

     /* Launch the rocket. */

     apply_thrust( use_blank_cols, rows );

     /* And we're done. */

     exit( EXIT_SUCCESS );
}

/* Define the clear_screen() function. */

void clear_screen( void )
{
     putchar( 27 );  /* ESC */
     putchar( 91 );  /* '[' */
     putchar( 49 );  /* '1' */
     putchar( 59 );  /* ';' */
     putchar( 49 );  /* '1' */
     putchar( 72 );  /* 'H' */
     putchar( 27 );  /* ESC */
     putchar( 91 );  /* '[' */
     putchar( 50 );  /* '2' */
     putchar( 74 );  /* 'J' */
     fflush( stdout );
     return;
}

/*

     C4droid does not use the environment variables LINES
     and COLUMNS so we will need to set them here.

     If your terminal emulator needs this function then
     use it to set the correct environment variable values
     for LINES and COLUMNS.

     Returns 0 for success and -1 for failure.

*/

#ifdef NEED_LINES_AND_COLUMNS

int set_lines_and_columns( void )
{
     int ret, save_errno;

     errno = 0;
     ret = setenv( "LINES", "46", 0 );  /* Use 46 lines. */
     if ( ret != 0 )
     {
          save_errno = errno;
          printf( "Failed to set the environment variable LINES.\n" );
          if ( save_errno != 0 )
          {
               printf( "Error: %s.\n", strerror( save_errno ) );
          }
          return ( -1 );
     }

     ret = setenv( "COLUMNS", "80", 0 );  /* Use 80 columns. */
     if ( ret != 0 )
     {
          save_errno = errno;
          printf( "Failed to set the environment variable COLUMNS.\n" );
          if ( save_errno != 0 )
          {
               printf( "Error: %s.\n", strerror( save_errno ) );
          }
          return ( -1 );
     }

     return 0;
}

#endif

void variable_delay( void )
{
     int current, ret, save_errno, start;
     static int calls = 0;
     struct timespec delay_length, remaining;

     calls++;
     start = 1000000000;  /* Delay in nanoseconds for the first call. */
     if ( calls == 1 )
     {
          current = start;
     }
     else
     {
          current = start / calls;
     }
     if ( current < 10000000 )
     {
          current = 10000000;
     }

     /* Set the structure variables. */

     delay_length.tv_sec = current / 1000000000;
     delay_length.tv_nsec = ( current -
                            ( delay_length.tv_sec * 1000000000 ) );

     remaining.tv_sec = 0;
     remaining.tv_nsec = 0;

     /* Start the delay. */

     do
     {
          errno = 0;
          ret = nanosleep( &delay_length, &remaining );
          if ( ret != 0 )
          {
               save_errno = errno;
               if ( save_errno != EINTR )
               {
                    printf( "\n\n\
Something went wrong when calling nanosleep(2).\n" );
                    if ( save_errno != 0 )
                    {
                         printf( "Error: %s.\n", strerror( save_errno ) );
                    }
                    printf( "\n" );
                    exit( EXIT_FAILURE );
               }
               else  /* Finish sleeping. */
               {
                    delay_length.tv_sec = remaining.tv_sec;
                    delay_length.tv_nsec = remaining.tv_nsec;
                    remaining.tv_sec = 0;
                    remaining.tv_nsec = 0;
               }    /* if ( save_errno != EINTR ) */
          }    /* if ( ret != 0 ) */
     }    while( ret != 0 );

     return;
}

/* This function prints the rocket, byte by byte. */

/*

                   /\
                  /  \
                 /    \
                /      \
               /        \
               |        |
               |        |
               |        |
               |  M     |
               |        |
               |    S   |    Matthew's Space Fleet
               |        |
               |      F |
               |        |
               |        |
               |        |
               |        |
               |        |
               |        |
              /          \
             /            \
            /______________\

*/

void print_rocket( int use_blank_cols )
{
     int count, count2, count3, count4, limit, spaces;

     /* Print the nose cone. */

     limit = use_blank_cols + 7;
     spaces = 0;
     for( count = 0; count < 5; count++, limit--, spaces += 2 )
     {
          for( count2 = 0; count2 < limit; count2++ )
          {
               putchar( 32 );  /* Space character */
          }
          putchar( 47 );  /* '/' */
          for( count2 = 0; count2 < spaces; count2++ )
          {
               putchar( 32 );  /* Space character */
          }
          putchar( 92 );  /* '\'  */
          putchar( 10 );  /* '\n' */
     }

     /* Print the body. */

     for( count = 0, limit++, spaces -= 2; count < 14; count++ )
     {
          for( count2 = 0; count2 < limit; count2++ )
          {
               putchar( 32 );  /* Space character */
          }
          putchar( 124 );  /* '|' */
          for( count2 = 0; count2 < spaces; count2++ )
          {
               if ( count == 3 && count2 == 2 )
               {
                    putchar( 77 );  /* 'M' */
               }
               else if ( count == 5 && count2 == 4 )
               {
                    putchar( 83 );  /* 'S' */
               }
               else if ( count == 7 && count2 == 6 )
               {
                    putchar( 70 );  /* 'F' */
               }
               else
               {
                    putchar( 32 );  /* Space character */
               }
          }
          putchar( 124 );  /* '|'  */
          putchar( 10 );   /* '\n' */
     }

     /* Print the base. */

     for( count = 0, limit--, spaces += 2; count < 2;
          count++, limit--, spaces += 2 )
     {
          for( count2 = 0; count2 < limit; count2++ )
          {
               putchar( 32 );  /* Space character */
          }
          putchar( 47 );  /* '/' */
          for( count2 = 0; count2 < spaces; count2++ )
          {
               putchar( 32 );  /* Space character */
          }
          putchar( 92 );  /* '\'  */
          putchar( 10 );  /* '\n' */
     }
     for( count = 0; count < limit; count++ )
     {
          putchar( 32 );  /* Space character */
     }
     putchar( 47 );  /* '/' */
     for( count = 0; count < spaces; count++ )
     {
          putchar( 95 );  /* '_' */
     }
     putchar( 92 );  /* '\'  */
     putchar( 10 );  /* '\n' */
     fflush( stdout );  /* Make sure everything gets printed. */

     /* And that's all. */

     return;
}

/* This function allows the rocket to blast off, line by line. */

void apply_thrust( int use_blank_cols, int rows )
{
     int count, count2, count3, limit, width;

     limit = use_blank_cols + 1;
     width = 14;
     for( count = 0; count < 7; count++, limit++, width -= 2 )
     {
          for( count2 = 0; count2 < 5; count2++ )
          {
               for( count3 = 0; count3 < limit; count3++ )
               {
                    putchar( 32 );  /* Space character */
               }
               for( count3 = 0; count3 < width; count3++ )
               {
                    putchar( 42 ); /* '*' */
               }
               putchar( 10 );  /* '\n' */
               fflush( stdout );  /* Make sure it gets printed.    */
               variable_delay();  /* Create a delay between lines. */
          }
     }

     /* Print blank lines to clear the screen as the rocket flies away. */

     for( count = 0; count < rows; count++ )
     {
          putchar( 10 );  /* '\n' */
          fflush( stdout );  /* This should happen anyway. */
          variable_delay();
     }

     /* And that's all. */

     return;
}

/* EOF rocket.c */