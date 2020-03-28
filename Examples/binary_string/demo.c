/*

     demo.c
     Written by Matthew Campbell.

*/

#include <stdio.h>
#include <string.h>

#include "binary_string.h"

int main( int argc, char **argv )
{
     int8_t byte;
     int16_t word;
     int32_t bits, dword;
     int64_t qword;

     /* Include zero bits in the binary_string. */

     binary_string_show_leading_zeros();

     /* Make sure we have a command line argument to use. */

     if ( argc != 3 )
     {
          printf( "Usage: demo <number of bits> <signed number>\n" );
          return 1;
     }

     /* Find out what data size to use. */

     if ( sscanf( argv[ 1 ], "%d", &bits ) != 1 )
     {
          printf( "That is not an valid number of bits.\n" );
          printf( "Usage: demo <number of bits> <signed number>\n" );
          return 1;
     }

     /* Read the signed number to be converted. */

     if ( sscanf( argv[ 2 ], "%lld", &qword ) != 1 )
     {
          printf( "That is not an acceptable signed number.\n" );
          printf( "Usage: demo <number of bits> <signed number>\n" );
          return 1;
     }

     /* sscanf(3) will return the maximum value if the number is too big. */

     if ( ( qword == 0x7FFFFFFFFFFFFFFF &&
            strcmp( argv[ 2 ], "9223372036854775807" ) != 0 ) ||
          ( qword == 0x8000000000000000 &&
            strcmp( argv[ 2 ], "-9223372036854775808" ) != 0 ) )
     {
          printf( "That is not an acceptable signed number.\n" );
          printf( "Usage: demo <number of bits> <signed number>\n" );
          return 1;
     }

     /* Make sure the number is in range for the data type and store it. */

     if ( bits == 8 )
     {
          if ( qword < ( -128LL ) || qword > 127LL )
          {
               printf( "Signed number is out of range for 8 bits.\n" );
               return 1;
          }
          byte = ( int8_t )qword;
     }
     else if ( bits == 16 )
     {
          if ( qword < ( -32768LL ) || qword > 32767LL )
          {
               printf( "Signed number is out of range for 16 bits.\n" );
               return 1;
          }
          word = ( int16_t )qword;
     }
     else if ( bits == 32 )
     {
          if ( qword < ( -2147483648LL ) || qword > 2147483647LL )
          {
               printf( "Signed number is out of range for 32 bits.\n" );
               return 1;
          }
          dword = ( int32_t )qword;
     }
     else if ( bits != 64 )
     {
          printf( "Invalid number of bits.\n" );
          return 1;
     }

     /* Convert the number to binary and print it. */

     if ( bits == 8 )
     {
          printf( "%s\n", int8_to_binary_string( byte ) );
     }
     else if ( bits == 16 )
     {
          printf( "%s\n", int16_to_binary_string( word ) );
     }
     else if ( bits == 32 )
     {
          printf( "%s\n", int32_to_binary_string( dword ) );
     }
     else  /* bits == 64 */
     {
          printf( "%s\n", int64_to_binary_string( qword ) );
     }

     return 0;
}

/* EOF demo.c */
