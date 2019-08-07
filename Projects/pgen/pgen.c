/*

     pgen.c
     Password Generator.
     Written by someone, sometime, somewhere.

     The author of this software shall not be held responsible for the
     user's use, misuse, or abuse of this software.  The user of this
     software assumes all risk and liability arising out of their use,
     misuse, or abuse of this software.  This software is provided
     "AS IS" without any warranty of any kind whatsoever.  The user
     that uses this software does so entirely at their own risk.

     This software is provided for educational purposes only and is
     not intended to be used for any illegal purpose.

     This program sends the output string to stdout using printf()
     instead of using a file pointer with fprintf() to eliminate
     one stack access operation.  This program has been optimized
     for speed.  All other output uses fprintf() to send the output
     to stderr which, unless redirected on the command line when
     running the program, will send such messages to the screen while
     allowing the user to send each output string to a file by
     redirecting stdout to a file when running this program from the
     command line.  This program could generate a file that is so
     large that the file system will be unable to handle it.  If this
     progam is run on a computer running something like Linux or Unix
     then the user could redirect stdout to a pipe file that had
     another program waiting to read the data.

     Using SUPPRESS_OUTPUT allows the user to gauge the performance
     of the program's logic apart from any delays associated with its
     output.  The time specified in the performance statistics may differ
     from how long it appears to take for the program to run if your
     device is not able to print the output fast enough to keep up with
     the program.  The program's output may be buffered by your operating
     system even when printing to the screen.

*/

#include <stdio.h>
#include <stdlib.h>

/* Define DEBUG if you want to print debugging output. */

#define DEBUG

/* Define SUPPRESS_OUTPUT if you don't want the output string printed. */

#undef  SUPPRESS_OUTPUT

/* Define SHOW_STATS if you want the performance statistics printed. */

#undef SHOW_STATS

#ifdef SHOW_STATS
#include <time.h>
#endif

/*

     Put the characters you want to use in your passwords in the choices
     array.  If you put them in in the correct order, the output will be
     sorted, otherwise it won't be.  The last entry in the choices array
     MUST be 0.  Specify the smallest and largest password lengths you
     would like to find.  You may use up to 63 characters in your
     passwords.  The space character is ASCII 32d.  The zero character
     is ASCII 48d.  There are 95 different 7 bit characters that can be
     typed from a common US keyboard.

*/

/*

char choices[] = { 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 0 };

*/

/* */

char choices[] = { 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45,
                   46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
                   60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73,
                   74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87,
                   88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100,
                   101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111,
                   112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122,
                   123, 124, 125, 126, 0 };

/* */

char str[ 64 ];

signed long int pos[ 64 ];

const signed long int smallest = 8;
const signed long int largest = 63;

int main( void )
{
     signed long int current, exit_inner_loop;
     signed long int exit_outer_loop, last, num_choices;
     register signed long int count;

#ifdef SHOW_STATS

     time_t seconds, start, stop;
     unsigned long long int acc, max, mem, total;

#endif

     /* Make sure we have values we can use. */

     if ( largest > 63 )
     {
          fprintf( stderr, "Invalid value for largest: %d.\n", largest );
          exit( EXIT_FAILURE );
     }
     if ( smallest < 1 || smallest > largest )
     {
          fprintf( stderr, "Invalid value for smallest: %d.\n", smallest );
          exit( EXIT_FAILURE );
     }

     /* Find out how many choices we have available. */

     num_choices = 0;
     while( choices[ num_choices ] != 0 && num_choices < 96 )
     {
          num_choices++;
     }

     /*

          It doesn't help to try the same password twice.  This can also
          happen if the last entry in the choices array is not null.

     */

     if ( num_choices == 96 )
     {
          fprintf( stderr, "Too many entries in the choices array.\n" );
          exit( EXIT_FAILURE );
     }

     /*

          We can't make any passwords if we
          don't have anything to work with.

     */

     if ( num_choices == 0 )
     {
          fprintf( stderr, "No choices available.\n" );
          exit( EXIT_FAILURE );
     }

#ifdef SHOW_STATS

     /* Make sure total won't overflow while the program is running. */

     /* max is max unsigned long long int */

     max = ( unsigned long long int )18446744073709551615;

     for( count = smallest; count <= largest; count++ )
     {
          acc = 1;
          for( current = 0; current < count; current++ )
          {
               mem = acc;
               acc = acc * ( unsigned long long int )num_choices;
               if ( mem > acc )
               {
                    fprintf( stderr, "\
(1) Data type overflow with %d characters in the output string.\n\
(1) To avoid this problem either undefine SHOW_STATS or change largest.\n",
                             count );
                    exit( EXIT_FAILURE );
               }
          }
          if ( max < acc )
          {
               fprintf( stderr, "\
(2) Data type overflow with %d characters in the output string.\n\
(2) To avoid this problem either undefine SHOW_STATS or change largest.\n",
                        count );
               exit( EXIT_FAILURE );
          }
          max = max - acc;
     }

#endif

     /* Clear the output string and the position array. */

     for( count = 63; count != 0; count-- )
     {
          str[ count ] = 0;
          pos[ count ] = -1;
     }
     str[ 0 ] = 0;
     pos[ 0 ] = -1;

#ifdef DEBUG

     fprintf( stderr, "(1) choices(%d): { ", ( num_choices + 1 ) );
     for( count = 0; count < num_choices; count++ )
     {
          fprintf( stderr, "\'%c\', ", choices[ count ] );
     }
     if ( choices[ count ] == 0 )
     {
          fprintf( stderr, "\'\\0\' }\n" );
     }
     else
     {
          fprintf( stderr, "\'%c\' }\n", choices[ count ] );
          fprintf( stderr, "The choices array does not end with null.\n" );
          exit( EXIT_FAILURE );
     }
     fprintf( stderr, "(2) smallest: %d, largest: %d.\n",
              smallest, largest );

#endif

     /* Load the output string and position array with initial values. */

     for( count = 0; count < smallest; count++ )
     {
          str[ count ] = choices[ 0 ];
          pos[ count ] = 0;
     }

     /*

        last is the array position of the
        last character in the output string.

     */

     last = smallest - 1;

     /* Adjust num_choices to be used as an array position. */

     num_choices--;

#ifdef DEBUG

     fprintf( stderr, "\
(3) Starting str: \"%s\", last: %d, num_choices: %d.\n",
              str, last, num_choices );

#endif

     exit_outer_loop = 0;

#ifdef SHOW_STATS

     total = 0;
     start = time( NULL );

#endif

     /* This is the part of the program that does the real work. */

     do
     {
          current = last;

#ifndef SUPPRESS_OUTPUT

          /* Print the current output string. */

          printf( "%s\n", str );

#endif

#ifdef SHOW_STATS

          total++;

#endif

          /* Increment if we can or roll it over and carry. */

          if ( pos[ current ] < num_choices )
          {
               ( pos[ current ] )++;
               str[ current ] = choices[ pos[ current ] ];
          }
          else  /* Roll over and carry. */
          {
               exit_inner_loop = 0;
               do
               {
                    if ( current == 0 )
                    {
                         /* If so, we're done. */

                         if ( ( last + 1 ) == largest )
                         {
                              exit_inner_loop = 1;
                              exit_outer_loop = 1;
                         }
                         else  /* Otherwise add another character. */
                         {
                              last++;
                              for( count = last; count != 0; count-- )
                              {
                                   str[ count ] = choices[ 0 ];
                                   pos[ count ] = 0;
                              }
                              str[ 0 ] = choices[ 0 ];
                              pos[ 0 ] = 0;
                              exit_inner_loop = 1;
                         }
                    }
                    else  /* current != 0 */
                    {
                         /* Roll over the current character. */

                         pos[ current ] = 0;
                         str[ current ] = choices[ 0 ];

                         /* Move to the previous character. */

                         current--;

                         /* Increment if we can. */

                         if ( pos[ current ] < num_choices )
                         {
                              ( pos[ current ] )++;
                              str[ current ] = choices[ pos[ current ] ];
                              exit_inner_loop = 1;
                         }
                    }    /* if ( current == 0 ) */
               }    while( exit_inner_loop == 0 );
          }    /* if ( pos[ last ] < num_choices ) */
     }    while( exit_outer_loop == 0 );

     /*

          Now we're done.  Show the performance
          statistics if requested and exit.

     */

#ifdef SHOW_STATS

     stop = time( NULL );
     seconds = stop - start;

     /*
          My compiler, C4droid, requires %llu
          to print an unsigned long long int.

     */

     fprintf( stderr, "total: %llu.\n", total );
     if ( seconds == 1 )
     {
          fprintf( stderr, "Time taken: 1 second.\n" );
     }
     else
     {
          fprintf( stderr, "Time taken: %d seconds.\n", seconds );
     }
     if ( seconds == 0 )
     {
          fprintf( stderr, "\
Insufficient runtime to determine approximate combinations per second.\n" );
     }
     else
     {
          fprintf( stderr, "Approximate combinations per second: %lu.\n",
                   ( unsigned long int )( total /
                   ( unsigned long long int )seconds ) );
     }

#endif

#ifdef SUPPRESS_OUTPUT

     /* Show something to let the user know we're done. */

     fprintf( stderr, "Program finished.\nExiting.\n" );

#endif

     exit( EXIT_SUCCESS );
}

/* EOF pgen.c */