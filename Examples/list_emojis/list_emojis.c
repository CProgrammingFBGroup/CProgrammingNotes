/*

     list_emojis.c
     Creates a list of 32-bit Unicode emoji pictograms.
     Written by Matthew Campbell.

*/

/*

     Everything from 0xF09F0000 to 0xF09FFFFF was checked to see
     what looked printable on my tablet.  Anything that didn't
     look like random garbage should be printed by this program,
     though some things will be printed more than once.

*/

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Prototype the emoji_to_string() function from the other file: */

uint8_t *emoji_to_string( uint32_t emoji, uint8_t *str );

/* Prototype the print_batch() function in this file: */

void print_batch( uint32_t start, uint32_t stop );

int main( void )
{
     /*

          The Unicode emojis do not appear to
          use a contiguous batch of numbers.

     */

     print_batch( 0xF09F0820, 0xF09F087E );

     print_batch( 0xF09F0D20, 0xF09F0D7E );

     print_batch( 0xF09F8084, 0xF09F8084 );

     print_batch( 0xF09F838F, 0xF09F838F );

     print_batch( 0xF09F8490, 0xF09F84BF );

     print_batch( 0xF09F8580, 0xF09F85AB );

     print_batch( 0xF09F85B0, 0xF09F85BF );

     print_batch( 0xF09F8680, 0xF09F869A );

     print_batch( 0xF09F87A6, 0xF09F87BF );

     print_batch( 0xF09F8880, 0xF09F8882 );

     print_batch( 0xF09F8890, 0xF09F88BA );

     print_batch( 0xF09F8980, 0xF09F8988 );

     print_batch( 0xF09F8990, 0xF09F8991 );

     print_batch( 0xF09F9080, 0xF09F90BE );

     print_batch( 0xF09F9180, 0xF09F9180 );

     print_batch( 0xF09F9182, 0xF09F91BF );

     print_batch( 0xF09F9280, 0xF09F92BF );

     print_batch( 0xF09F9380, 0xF09F93B7 );

     print_batch( 0xF09F93B9, 0xF09F93BC );

     print_batch( 0xF09F9480, 0xF09F94BD );

     print_batch( 0xF09F9590, 0xF09F95A7 );

     print_batch( 0xF09F97BB, 0xF09F97BF );

     print_batch( 0xF09F9880, 0xF09F98BF );

     print_batch( 0xF09F9980, 0xF09F9980 );

     print_batch( 0xF09F9985, 0xF09F998F );

     exit( EXIT_SUCCESS );
}

/* This function prints a batch of emoji pictograms. */

void print_batch( uint32_t start, uint32_t stop )
{
     int save_errno;
     uint32_t emoji;
     uint8_t *ptr, str[ 5 ];

     /* Make sure we have useful start and stop values. */

     if ( start == 0 )
     {
          printf( "print_batch(): Invalid start value.\n\n" );
          exit( EXIT_FAILURE );
     }

     if ( stop == 0 || stop < start )
     {
          printf( "print_batch(): Invalid stop value.\n\n" );
          exit( EXIT_FAILURE );
     }

     for( emoji = start; emoji <= stop; emoji++ )
     {
          errno = 0;
          ptr = emoji_to_string( emoji, str );
          save_errno = errno;

          if ( ptr == NULL )
          {
               printf( "\
Something went wrong while converting the emoji to a printable string.\n" );
               if ( save_errno != 0 )
               {
                    printf( "Error: %s.\n", strerror( save_errno ) );
               }
               printf( "\nProgram failed.  Exiting.\n\n" );
               exit( EXIT_FAILURE );
          }

          printf( "%s uses 0x%08X\n", ptr, emoji );

     }    /* for( emoji = start; emoji <= stop; emoji++ ); */

     return;
}

/* EOF list_emojis.c */

