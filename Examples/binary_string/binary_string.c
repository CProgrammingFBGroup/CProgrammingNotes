/*

     binary_string.c

     This file contains the following functions:

                void  binary_string_show_leading_zeros( void );
                void  binary_string_suppress_leading_zeros( void );

          const char *int8_to_binary_string( const char num );
          const char *int16_to_binary_string( const short int num );
          const char *int32_to_binary_string( const long int num );
          const char *int64_to_binary_string( const long long int num );

          int binary_string_to_int8( const char *str, char *num );
          int binary_string_to_int16( const char *str, short int *num );
          int binary_string_to_int32( const char *str, long int *num );
          int binary_string_to_int64( const char *str, long long int *num );

     Written by Matthew Campbell.  (12-15-17)

     It took 5 days to write this file and get rid of all the bugs.

     It has 1,476 lines of code.  It's just a boring box of nuts
     and bolts, but a must have for anyone that works with bits.

*/

#ifndef _BINARY_STRING_C
#define _BINARY_STRING_C

#include <stdio.h>

/*

     This is the global variable that is used to indicate
     whether leading bytes that are all zeros should be
     shown or suppressed when returning binary strings.

     It must be either 1 or 0.

*/

static int binary_string_show_zeros = 0;

/*

     This function sets the global variable to
     1 so all leading all zero bytes are shown.

*/

void binary_string_show_leading_zeros()
{
     binary_string_show_zeros = 1;
     return;
}

/*

     This function clears the global variable to 0
     so all leading all zero bytes are suppressed.

*/

void binary_string_suppress_leading_zeros( void )
{
     binary_string_show_zeros = 0;
     return;
}

/*

     This function converts an 8 bit integer
     into a character string of 1's and 0's.

     Returns a static character string expessing num in binary form.

*/

const char *int8_to_binary_string( const char num )
{
     static char str[ 9 ];
     char pos;
     int count;

     if ( binary_string_show_zeros != 0 && binary_string_show_zeros != 1 )
     {
          binary_string_show_zeros = 0;
     }
     if ( num < 0 )  /* Use the sign bit. */
     {
          str[ 0 ] = '1';
     }
     else
     {
          str[ 0 ] = '0';
     }
     for( count = 1, pos = 64; count < 8; count++, pos /= 2 )
     {
          if ( num & pos )
          {
               str[ count ] = '1';
          }
          else
          {
               str[ count ] = '0';
          }
     }
     str[ count ] = 0;
     return str;
}

/*

     This function converts a 16 bit integer
     into a character string of 1's and 0's.

     Returns a static character string expessing num in binary form
     most significant byte first and least significant byte last.

*/

const char *int16_to_binary_string( const short int num )
{
     static char str[ 18 ];
     int count;
     short int pos;

     if ( binary_string_show_zeros !=0 && binary_string_show_zeros != 1 )
     {
          binary_string_show_zeros = 0;
     }
     if ( binary_string_show_zeros == 1 )
     {
          if ( num < 0 )  /* Use the sign bit. */
          {
               str[ 0 ] = '1';
          }
          else
          {
               str[ 0 ] = '0';
          }
          for( count = 1, pos = 16384; count < 8; count++, pos /= 2 )
          {
               if ( num & pos )
               {
                    str[ count ] = '1';
               }
               else
               {
                    str[ count ] = '0';
               }
          }
          str[ count ] = 32;
          for( count++; count < 17; count++, pos /= 2 )
          {
               if ( num & pos )
               {
                    str[ count ] = '1';
               }
               else
               {
                    str[ count ] = '0';
               }
          }
          str[ count ] = 0;
     }
     else  /*  binary_string_show_zeros == 0 */
     {
          if ( num < 0 || num >= 256 )
          {
               if ( num < 0 )  /* Use the sign bit. */
               {
                    str[ 0 ] = '1';
               }
               else
               {
                    str[ 0 ] = '0';
               }
               for( count = 1, pos = 16384; count < 8;
                    count++, pos /= 2 )
               {
                    if ( num & pos )
                    {
                         str[ count ] = '1';
                    }
                    else
                    {
                         str[ count ] = '0';
                    }
               }
               str[ count ] = 32;
               for( count++; count < 17; count++, pos /= 2 )
               {
                    if ( num & pos )
                    {
                         str[ count ] = '1';
                    }
                    else
                    {
                         str[ count ] = '0';
                    }
               }
               str[ count ] = 0;
          }
          else  /* num >= 0 && num < 256 */
          {
               for( count = 0, pos = 128; count < 8; count++, pos /= 2 )
               {
                    if ( num & pos )
                    {
                         str[ count ] = '1';
                    }
                    else
                    {
                         str[ count ] = '0';
                    }
               }
               str[ count ] = 0;
          }

     }
     return str;
}

/*

     This function converts a 32 bit integer
     into a character string of 1's and 0's.

     Returns a static character string expessing num in binary form
     most significant byte first and least significant byte last.

*/

const char *int32_to_binary_string( const long int num )
{
     static char str[ 36 ];
     int count;
     long int pos;

     if ( binary_string_show_zeros !=0 &&
          binary_string_show_zeros != 1 )
     {
          binary_string_show_zeros = 0;
     }
     if ( binary_string_show_zeros == 1 )
     {
          if ( num < 0 )  /* Use the sign bit. */
          {
               str[ 0 ] = '1';
          }
          else
          {
               str[ 0 ] = '0';
          }
          for( count = 1, pos = 1073741824; count < 8;
               count++, pos /= 2 )
          {
               if ( num & pos )
               {
                    str[ count ] = '1';
               }
               else
               {
                    str[ count ] = '0';
               }
          }
          str[ count ] = 32;
          for( count++; count < 17; count++, pos /= 2 )
          {
               if ( num & pos )
               {
                    str[ count ] = '1';
               }
               else
               {
                    str[ count ] = '0';
               }
          }
          str[ count ] = 32;
          for( count++; count < 26; count++, pos /= 2 )
          {
               if ( num & pos )
               {
                    str[ count ] = '1';
               }
               else
               {
                    str[ count ] = '0';
               }
          }
          str[ count ] = 32;
          for( count++; count < 35; count++, pos /= 2 )
          {
               if ( num & pos )
               {
                    str[ count ] = '1';
               }
               else
               {
                    str[ count ] = '0';
               }
          }
          str[ count ] = 0;
     }
     else  /* binary_string_show_zeros == 0 */
     {
          if ( num < 0 || num >= 16777216 )
          {
               if ( num < 0 )  /* Use the sign bit. */
               {
                    str[ 0 ] = '1';
               }
               else
               {
                    str[ 0 ] = '0';
               }
               for( count = 1, pos = 1073741824; count < 8;
                    count++, pos /= 2 )
               {
                    if ( num & pos )
                    {
                         str[ count ] = '1';
                    }
                    else
                    {
                         str[ count ] = '0';
                    }
               }
               str[ count ] = 32;
               for( count++; count < 17; count++, pos /= 2 )
               {
                    if ( num & pos )
                    {
                         str[ count ] = '1';
                    }
                    else
                    {
                         str[ count ] = '0';
                    }
               }
               str[ count ] = 32;
               for( count++; count < 26; count++, pos /= 2 )
               {
                    if ( num & pos )
                    {
                         str[ count ] = '1';
                    }
                    else
                    {
                         str[ count ] = '0';
                    }
               }
               str[ count ] = 32;
               for( count++; count < 35; count++, pos /= 2 )
               {
                    if ( num & pos )
                    {
                         str[ count ] = '1';
                    }
                    else
                    {
                         str[ count ] = '0';
                    }
               }
               str[ count ] = 0;
          }
          else if ( num < 256 )
          {
               for( count = 0, pos = 128; count < 8;
                    count++, pos /= 2 )
               {
                    if ( num & pos )
                    {
                         str[ count ] = '1';
                    }
                    else
                    {
                         str[ count ] = '0';
                    }
               }
               str[ count ] = 0;
          }
          else if ( num < 65536 )
          {
               for( count = 0, pos = 32768; count < 8;
                    count++, pos /= 2 )
               {
                    if ( num & pos )
                    {
                         str[ count ] = '1';
                    }
                    else
                    {
                         str[ count ] = '0';
                    }
               }
               str[ count ] = 32;
               for( count++; count < 17; count++, pos /= 2 )
               {
                    if ( num & pos )
                    {
                         str[ count ] = '1';
                    }
                    else
                    {
                         str[ count ] = '0';
                    }
               }
               str[ count ] = 0;
          }
          else  /* num < 16777216 */
          {
               for( count = 0, pos = 8388608; count < 8;
                    count++, pos /= 2 )
               {
                    if ( num & pos )
                    {
                         str[ count ] = '1';
                    }
                    else
                    {
                         str[ count ] = '0';
                    }
               }
               str[ count ] = 32;
               for( count++; count < 17; count++, pos /= 2 )
               {
                    if ( num & pos )
                    {
                         str[ count ] = '1';
                    }
                    else
                    {
                         str[ count ] = '0';
                    }
               }
               str[ count ] = 32;
               for( count++; count < 26; count++, pos /= 2 )
               {
                    if ( num & pos )
                    {
                         str[ count ] = '1';
                    }
                    else
                    {
                         str[ count ] = '0';
                    }
               }
               str[ count ] = 0;
          }
     }
     return str;
}

/*

     This function converts a 64 bit integer
     into a character string of 1's and 0's.

     Returns a static character string expessing num in binary form
     most significant byte first and least significant byte last.

*/

const char *int64_to_binary_string( const long long int num )
{
     static char str[ 72 ];
     int count;
     long long int pos;

     if ( binary_string_show_zeros != 0 &&
          binary_string_show_zeros != 1 )
     {
          binary_string_show_zeros = 0;
     }
     if ( binary_string_show_zeros == 1 )
     {
          if ( num < 0 )  /* Use the sign bit. */
          {
               str[ 0 ] = '1';
          }
          else
          {
               str[ 0 ] = '0';
          }
          for( count = 1, pos = 4611686018427387904; count < 8;
               count++, pos /= 2 )
          {
               if ( num & pos )
               {
                    str[ count ] = '1';
               }
               else
               {
                    str[ count ] = '0';
               }
          }
          str[ count ] = 32;
          for( count++; count < 17; count++, pos /= 2 )
          {
               if ( num & pos )
               {
                    str[ count ] = '1';
               }
               else
               {
                    str[ count ] = '0';
               }
          }
          str[ count ] = 32;
          for( count++; count < 26; count++, pos /= 2 )
          {
               if ( num & pos )
               {
                    str[ count ] = '1';
               }
               else
               {
                    str[ count ] = '0';
               }
          }
          str[ count ] = 32;
          for( count++; count < 35; count++, pos /= 2 )
          {
               if ( num & pos )
               {
                    str[ count ] = '1';
               }
               else
               {
                    str[ count ] = '0';
               }
          }
          str[ count ] = 32;
          for( count++; count < 44; count++, pos /= 2 )
          {
               if ( num & pos )
               {
                    str[ count ] = '1';
               }
               else
               {
                    str[ count ] = '0';
               }
          }
          str[ count ] = 32;
          for( count++; count < 53; count++, pos /= 2 )
          {
               if ( num & pos )
               {
                    str[ count ] = '1';
               }
               else
               {
                    str[ count ] = '0';
               }
          }
          str[ count ] = 32;
          for( count++; count < 62; count++, pos /= 2 )
          {
               if ( num & pos )
               {
                    str[ count ] = '1';
               }
               else
               {
                    str[ count ] = '0';
               }
          }
          str[ count ] = 32;
          for( count++; count < 71; count++, pos /= 2 )
          {
               if ( num & pos )
               {
                    str[ count ] = '1';
               }
               else
               {
                    str[ count ] = '0';
               }
          }
          str[ count ] = 0;
     }
     else  /* binary_string_show_zeros == 0 */
     {
          if ( num < 0 || num >= 72057594037927936 )
          {
               if ( num < 0 )  /* Use the sign bit. */
               {
                    str[ 0 ] = '1';
               }
               else
               {
                    str[ 0 ] = '0';
               }
               for( count = 1, pos = 4611686018427387904; count < 8;
                    count++, pos /= 2 )
               {
                    if ( num & pos )
                    {
                         str[ count ] = '1';
                    }
                    else
                    {
                         str[ count ] = '0';
                    }
               }
               str[ count ] = 32;
               for( count++; count < 17; count++, pos /= 2 )
               {
                    if ( num & pos )
                    {
                         str[ count ] = '1';
                    }
                    else
                    {
                         str[ count ] = '0';
                    }
               }
               str[ count ] = 32;
               for( count++; count < 26; count++, pos /= 2 )
               {
                    if ( num & pos )
                    {
                         str[ count ] = '1';
                    }
                    else
                    {
                         str[ count ] = '0';
                    }
               }
               str[ count ] = 32;
               for( count++; count < 35; count++, pos /= 2 )
               {
                    if ( num & pos )
                    {
                         str[ count ] = '1';
                    }
                    else
                    {
                         str[ count ] = '0';
                    }
               }
               str[ count ] = 32;
               for( count++; count < 44; count++, pos /= 2 )
               {
                    if ( num & pos )
                    {
                         str[ count ] = '1';
                    }
                    else
                    {
                         str[ count ] = '0';
                    }
               }
               str[ count ] = 32;
               for( count++; count < 53; count++, pos /= 2 )
               {
                    if ( num & pos )
                    {
                         str[ count ] = '1';
                    }
                    else
                    {
                         str[ count ] = '0';
                    }
               }
               str[ count ] = 32;
               for( count++; count < 62; count++, pos /= 2 )
               {
                    if ( num & pos )
                    {
                         str[ count ] = '1';
                    }
                    else
                    {
                         str[ count ] = '0';
                    }
               }
               str[ count ] = 32;
               for( count++; count < 71; count++, pos /= 2 )
               {
                    if ( num & pos )
                    {
                         str[ count ] = '1';
                    }
                    else
                    {
                         str[ count ] = '0';
                    }
               }
               str[ count ] = 0;
          }
          else if ( num < 256 )
          {
               for( count = 0, pos = 128; count < 8;
                    count++, pos /= 2 )
               {
                    if ( num & pos )
                    {
                         str[ count ] = '1';
                    }
                    else
                    {
                         str[ count ] = '0';
                    }
               }
               str[ count ] = 0;
          }
          else if ( num < 65536 )
          {
               for( count = 0, pos = 32768; count < 8;
                    count++, pos /= 2 )
               {
                    if ( num & pos )
                    {
                         str[ count ] = '1';
                    }
                    else
                    {
                         str[ count ] = '0';
                    }
               }
               str[ count ] = 32;
               for( count++; count < 17; count++, pos /= 2 )
               {
                    if ( num & pos )
                    {
                         str[ count ] = '1';
                    }
                    else
                    {
                         str[ count ] = '0';
                    }
               }
               str[ count ] = 0;
          }
          else if ( num < 16777216 )
          {
               for( count = 0, pos = 8388608; count < 8;
                    count++, pos /= 2 )
               {
                    if ( num & pos )
                    {
                         str[ count ] = '1';
                    }
                    else
                    {
                         str[ count ] = '0';
                    }
               }
               str[ count ] = 32;
               for( count++; count < 17; count++, pos /= 2 )
               {
                    if ( num & pos )
                    {
                         str[ count ] = '1';
                    }
                    else
                    {
                         str[ count ] = '0';
                    }
               }
               str[ count ] = 32;
               for( count++; count < 26; count++, pos /= 2 )
               {
                    if ( num & pos )
                    {
                         str[ count ] = '1';
                    }
                    else
                    {
                         str[ count ] = '0';
                    }
               }
               str[ count ] = 0;
          }
          else if ( num < 4294967296 )
          {
               for( count = 0, pos = 2147483648; count < 8;
                    count++, pos /= 2 )
               {
                    if ( num & pos )
                    {
                         str[ count ] = '1';
                    }
                    else
                    {
                         str[ count ] = '0';
                    }
               }
               str[ count ] = 32;
               for( count++; count < 17; count++, pos /= 2 )
               {
                    if ( num & pos )
                    {
                         str[ count ] = '1';
                    }
                    else
                    {
                         str[ count ] = '0';
                    }
               }
               str[ count ] = 32;
               for( count++; count < 26; count++, pos /= 2 )
               {
                    if ( num & pos )
                    {
                         str[ count ] = '1';
                    }
                    else
                    {
                         str[ count ] = '0';
                    }
               }
               str[ count ] = 32;
               for( count++; count < 35; count++, pos /= 2 )
               {
                    if ( num & pos )
                    {
                         str[ count ] = '1';
                    }
                    else
                    {
                         str[ count ] = '0';
                    }
               }
               str[ count ] = 0;
          }
          else if ( num < 1099511627776 )
          {
               for( count = 0, pos = 549755813888; count < 8;
                    count++, pos /= 2 )
               {
                    if ( num & pos )
                    {
                         str[ count ] = '1';
                    }
                    else
                    {
                         str[ count ] = '0';
                    }
               }
               str[ count ] = 32;
               for( count++; count < 17; count++, pos /= 2 )
               {
                    if ( num & pos )
                    {
                         str[ count ] = '1';
                    }
                    else
                    {
                         str[ count ] = '0';
                    }
               }
               str[ count ] = 32;
               for( count++; count < 26; count++, pos /= 2 )
               {
                    if ( num & pos )
                    {
                         str[ count ] = '1';
                    }
                    else
                    {
                         str[ count ] = '0';
                    }
               }
               str[ count ] = 32;
               for( count++; count < 35; count++, pos /= 2 )
               {
                    if ( num & pos )
                    {
                         str[ count ] = '1';
                    }
                    else
                    {
                         str[ count ] = '0';
                    }
               }
               str[ count ] = 32;
               for( count++; count < 44; count++, pos /= 2 )
               {
                    if ( num & pos )
                    {
                         str[ count ] = '1';
                    }
                    else
                    {
                         str[ count ] = '0';
                    }
               }
               str[ count ] = 0;
          }
          else if ( num < 281474976710656 )
          {
               for( count = 0, pos = 140737488355328; count < 8;
                    count++, pos /= 2 )
               {
                    if ( num & pos )
                    {
                         str[ count ] = '1';
                    }
                    else
                    {
                         str[ count ] = '0';
                    }
               }
               str[ count ] = 32;
               for( count++; count < 17; count++, pos /= 2 )
               {
                    if ( num & pos )
                    {
                         str[ count ] = '1';
                    }
                    else
                    {
                         str[ count ] = '0';
                    }
               }
               str[ count ] = 32;
               for( count++; count < 26; count++, pos /= 2 )
               {
                    if ( num & pos )
                    {
                         str[ count ] = '1';
                    }
                    else
                    {
                         str[ count ] = '0';
                    }
               }
               str[ count ] = 32;
               for( count++; count < 35; count++, pos /= 2 )
               {
                    if ( num & pos )
                    {
                         str[ count ] = '1';
                    }
                    else
                    {
                         str[ count ] = '0';
                    }
               }
               str[ count ] = 32;
               for( count++; count < 44; count++, pos /= 2 )
               {
                    if ( num & pos )
                    {
                         str[ count ] = '1';
                    }
                    else
                    {
                         str[ count ] = '0';
                    }
               }
               str[ count ] = 32;
               for( count++; count < 53; count++, pos /= 2 )
               {
                    if ( num & pos )
                    {
                         str[ count ] = '1';
                    }
                    else
                    {
                         str[ count ] = '0';
                    }
               }
               str[ count ] = 0;
          }
          else  /* num < 72057594037927936 */
          {
               for( count = 0, pos = 36028797018963968; count < 8;
                    count++, pos /= 2 )
               {
                    if ( num & pos )
                    {
                         str[ count ] = '1';
                    }
                    else
                    {
                         str[ count ] = '0';
                    }
               }
               str[ count ] = 32;
               for( count++; count < 17; count++, pos /= 2 )
               {
                    if ( num & pos )
                    {
                         str[ count ] = '1';
                    }
                    else
                    {
                         str[ count ] = '0';
                    }
               }
               str[ count ] = 32;
               for( count++; count < 26; count++, pos /= 2 )
               {
                    if ( num & pos )
                    {
                         str[ count ] = '1';
                    }
                    else
                    {
                         str[ count ] = '0';
                    }
               }
               str[ count ] = 32;
               for( count++; count < 35; count++, pos /= 2 )
               {
                    if ( num & pos )
                    {
                         str[ count ] = '1';
                    }
                    else
                    {
                         str[ count ] = '0';
                    }
               }
               str[ count ] = 32;
               for( count++; count < 44; count++, pos /= 2 )
               {
                    if ( num & pos )
                    {
                         str[ count ] = '1';
                    }
                    else
                    {
                         str[ count ] = '0';
                    }
               }
               str[ count ] = 32;
               for( count++; count < 53; count++, pos /= 2 )
               {
                    if ( num & pos )
                    {
                         str[ count ] = '1';
                    }
                    else
                    {
                         str[ count ] = '0';
                    }
               }
               str[ count ] = 32;
               for( count++; count < 62; count++, pos /= 2 )
               {
                    if ( num & pos )
                    {
                         str[ count ] = '1';
                    }
                    else
                    {
                         str[ count ] = '0';
                    }
               }
               str[ count ] = 0;
          }
     }
     return str;
}

/*

     This function reads a character string of '0's and '1's
     which may contain zero or more spaces between bits, but
     must contain 8 bits along with a null '\0' at the end
     of the string.

     Returns 0 for success and -1 for failure.
     The result is stored in num.

*/

int binary_string_to_int8( const char *str, char *num )
{
     char bit_str[ 9 ], build_num, exit_loop, factor;
     int bit_pos;
     long int str_pos;

     if ( str == NULL || num == NULL )
     {
          return ( -1 );  /* Null pointers are invalid arguments. */
     }
     if ( str[ 0 ] == 0 )
     {
          return ( -1 );  /* An empty string is an invalid argument. */
     }
     bit_pos = 0;
     build_num = exit_loop = 0;
     factor = 1;
     str_pos = 0;
     do
     {
          while( str[ str_pos ] == 32 )
          {
               str_pos++;  /* Skip any space characters. */
          }
          if ( str[ str_pos ] == 0 )
          {
               if ( bit_pos != 8 )
               {
                    return ( -1 );  /* We don't have a complete byte. */
               }
               else
               {
                    exit_loop = 1;  /* We're done gathering the bits. */
               }
          }
          else if ( str[ str_pos ] == '0' )
          {
               if ( bit_pos == 8 )
               {
                    return ( -1 );  /* Too many bits. */
               }
               else
               {
                    bit_str[ bit_pos ] = '0';
                    bit_pos++;
                    str_pos++;
               }
          }
          else if ( str[ str_pos ] == '1' )
          {
               if ( bit_pos == 8 )
               {
                    return ( -1 );  /* Too many bits. */
               }
               else
               {
                    bit_str[ bit_pos ] = '1';
                    bit_pos++;
                    str_pos++;
               }
          }
          else  /* Invalid character in user supplied string. */
          {
               return ( -1 );
          }
     }    while( exit_loop == 0 );

     /* Build up the number. */

     bit_pos--;
     do
     {
          if ( bit_str[ bit_pos ] == '1' )
          {
               build_num = build_num + factor;
          }
          bit_pos--;
          factor *= 2;
     }    while( bit_pos != ( -1 ) );

     /* Now store the number and we're done. */

     *num = build_num;
     return 0;
}

/*

     This function reads a character string of '0's and '1's
     which may contain zero or more spaces between bits, but
     must contain 8 or 16 bits along with a null '\0' at the
     end of the string.

     Returns 0 for success and -1 for failure.
     The result is stored in num.

*/

int binary_string_to_int16( const char *str, short int *num )
{
     char bit_str[ 17 ], exit_loop;
     int bit_pos;
     short int build_num, factor;
     long int str_pos;

     if ( str == NULL || num == NULL )
     {
          return ( -1 );  /* Null pointers are invalid arguments. */
     }
     if ( str[ 0 ] == 0 )
     {
          return ( -1 );  /* An empty string is an invalid argument. */
     }
     bit_pos = 0;
     exit_loop = 0;
     build_num = 0;
     factor = 1;
     str_pos = 0;
     do
     {
          while( str[ str_pos ] == 32 )
          {
               str_pos++;  /* Skip any space characters. */
          }
          if ( str[ str_pos ] == 0 )
          {
               if ( bit_pos != 8 && bit_pos != 16 )
               {
                    return ( -1 );  /* We don't have a complete byte. */
               }
               else
               {
                    exit_loop = 1;  /* We're done gathering the bits. */
               }
          }
          else if ( str[ str_pos ] == '0' )
          {
               if ( bit_pos == 16 )
               {
                    return ( -1 );  /* Too many bits. */
               }
               else
               {
                    bit_str[ bit_pos ] = '0';
                    bit_pos++;
                    str_pos++;
               }
          }
          else if ( str[ str_pos ] == '1' )
          {
               if ( bit_pos == 16 )
               {
                    return ( -1 );  /* Too many bits. */
               }
               else
               {
                    bit_str[ bit_pos ] = '1';
                    bit_pos++;
                    str_pos++;
               }
          }
          else  /* Invalid character in user supplied string. */
          {
               return ( -1 );
          }
     }    while( exit_loop == 0 );

     /* Build up the number. */

     /* If we received less than 2 bytes then the */
     /* leading bits will be presumed to be zero. */

     bit_pos--;
     do
     {
          if ( bit_str[ bit_pos ] == '1' )
          {
               build_num = build_num + factor;
          }
          bit_pos--;
          factor *= 2;
     }    while( bit_pos != ( -1 ) );

     /* Now store the number and we're done. */

     *num = build_num;
     return 0;
}

/*

     This function reads a character string of '0's and '1's
     which may contain zero or more spaces between bits, but
     must contain 8, 16, 24, or 32 bits along with a null '\0'
     at the end of the string.

     Returns 0 for success and -1 for failure.
     The result is stored in num.

*/

int binary_string_to_int32( const char *str, long int *num )
{
     char bit_str[ 33 ], exit_loop;
     int bit_pos;
     long int build_num, factor, str_pos;

     if ( str == NULL || num == NULL )
     {
          return ( -1 );  /* Null pointers are invalid arguments. */
     }
     if ( str[ 0 ] == 0 )
     {
          return ( -1 );  /* An empty string is an invalid argument. */
     }
     bit_pos = 0;
     exit_loop = 0;
     build_num = str_pos = 0;
     factor = 1;
     do
     {
          while( str[ str_pos ] == 32 )
          {
               str_pos++;  /* Skip any space characters. */
          }
          if ( str[ str_pos ] == 0 )
          {
               if ( bit_pos != 8 && bit_pos != 16 &&
                    bit_pos != 24 && bit_pos != 32 )
               {
                    return ( -1 );  /* We don't have a complete byte. */
               }
               else
               {
                    exit_loop = 1;  /* We're done gathering the bits. */
               }
          }
          else if ( str[ str_pos ] == '0' )
          {
               if ( bit_pos == 32 )
               {
                    return ( -1 );  /* Too many bits. */
               }
               else
               {
                    bit_str[ bit_pos ] = '0';
                    bit_pos++;
                    str_pos++;
               }
          }
          else if ( str[ str_pos ] == '1' )
          {
               if ( bit_pos == 32 )
               {
                    return ( -1 );  /* Too many bits. */
               }
               else
               {
                    bit_str[ bit_pos ] = '1';
                    bit_pos++;
                    str_pos++;
               }
          }
          else  /* Invalid character in user supplied string. */
          {
               return ( -1 );
          }
     }    while( exit_loop == 0 );

     /* Build up the number. */

     /* If we received less than 4 bytes then the */
     /* leading bits will be presumed to be zero. */

     bit_pos--;
     do
     {
          if ( bit_str[ bit_pos ] == '1' )
          {
               build_num = build_num + factor;
          }
          bit_pos--;
          factor *= 2;
     }    while( bit_pos != ( -1 ) );

     /* Now store the number and we're done. */

     *num = build_num;
     return 0;
}

/*

     This function reads a character string of '0's and '1's
     which may contain zero or more spaces between bits, but
     must contain 8, 16, 24, 32, 40, 48, 56, or 64 bits along
     with a null '\0' at the end of the string.

     Returns 0 for success and -1 for failure.
     The result is stored in num.

*/

int binary_string_to_int64( const char *str, long long int *num )
{
     char bit_str[ 65 ], exit_loop;
     int bit_pos;
     long long int build_num, factor;
     long int str_pos;

     if ( str == NULL || num == NULL )
     {
          return ( -1 );  /* Null pointers are invalid arguments. */
     }
     if ( str[ 0 ] == 0 )
     {
          return ( -1 );  /* An empty string is an invalid argument. */
     }
     bit_pos = 0;
     exit_loop = 0;
     build_num = 0;
     str_pos = 0;
     factor = 1;
     do
     {
          while( str[ str_pos ] == 32 )
          {
               str_pos++;  /* Skip any space characters. */
          }
          if ( str[ str_pos ] == 0 )
          {
               if ( bit_pos != 8 && bit_pos != 16 &&
                    bit_pos != 24 && bit_pos != 32 &&
                    bit_pos != 40 && bit_pos != 48 &&
                    bit_pos != 56 && bit_pos != 64 )
               {
                    return ( -1 );  /* We don't have a complete byte. */
               }
               else
               {
                    exit_loop = 1;  /* We're done gathering the bits. */
               }
          }
          else if ( str[ str_pos ] == '0' )
          {
               if ( bit_pos == 64 )
               {
                    return ( -1 );  /* Too many bits. */
               }
               else
               {
                    bit_str[ bit_pos ] = '0';
                    bit_pos++;
                    str_pos++;
               }
          }
          else if ( str[ str_pos ] == '1' )
          {
               if ( bit_pos == 64 )
               {
                    return ( -1 );  /* Too many bits. */
               }
               else
               {
                    bit_str[ bit_pos ] = '1';
                    bit_pos++;
                    str_pos++;
               }
          }
          else  /* Invalid character in user supplied string. */
          {
               return ( -1 );
          }
     }    while( exit_loop == 0 );

     /* Build up the number. */

     /* If we received less than 8 bytes then the */
     /* leading bits will be presumed to be zero. */

     bit_pos--;
     do
     {
          if ( bit_str[ bit_pos ] == '1' )
          {
               build_num = build_num + factor;
          }
          bit_pos--;
          factor *= 2;
     }    while( bit_pos != ( -1 ) );

     /* Now store the number and we're done. */

     *num = build_num;
     return 0;
}

#endif  /* _BINARY_STRING_C */

/* EOF binary_string.c */
