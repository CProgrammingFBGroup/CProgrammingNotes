/*

     look_at_bits.c
     Written by Matthew Campbell.

*/

#include <stdio.h>

#define USAGE "Usage: look_at_bits <floating point number>\n"

int main( int argc, char **argv )
{
     uint32_t bit32, num32;
     uint64_t bit64, num64;

     union _box32
     {
          float    fnum;
          uint32_t inum;
     } box32;

     union _box64
     {
          double   fnum;
          uint64_t inum;
     } box64;

     /* Make sure we have the correct number of command line arguments. */

     if ( argc != 2 )
     {
          printf( "%s", USAGE );
          return 1;
     }

     /* Read the command line argument. */

     if ( sscanf( argv[ 1 ], "%f", &box32.fnum ) != 1 )
     {
          printf( "%s", USAGE );
          return 1;
     }

     if ( sscanf( argv[ 1 ], "%lf", &box64.fnum ) != 1 )
     {
          printf( "%s", USAGE );
          return 1;
     }

     /* Copy out the bits to integers. */

     num32 = box32.inum;
     num64 = box64.inum;

    /* Set the bit to test. */

     bit32 = ( uint32_t )0x80000000;
     bit64 = ( uint64_t )0x8000000000000000;

     /* Print the 32 bit floating point number. */

     printf( "%.0f as float:  ", box32.fnum );
     while( bit32 > 0 )
     {
          if ( bit32 & num32 )
          {
               printf( "1" );
          }
          else
          {
               printf( "0" );
          }
          bit32 = bit32 >> 1;
     }
     printf( "\n" );

     /* Print the 64 bit floating point number. */

     printf( "%.0lf as double: ", box64.fnum );
     while( bit64 > 0 )
     {
          if ( bit64 & num64 )
          {
               printf( "1" );
          }
          else
          {
               printf( "0" );
          }
          bit64 = bit64 >> 1;
     }
     printf( "\n\n" );

     /* And we're done. */

     return 0;
}

/* EOF */
