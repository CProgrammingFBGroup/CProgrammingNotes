/*

     multiple.c
     Multiplies a numer times itself s high as it can.
     Written by Matthew Campbell on Wednesday January 2, 2019.

*/

#include <stdio.h>

/* Undefine DEBUG if you don't want debugging output. */

#define DEBUG

int main( void )
{
     int count;
     unsigned long long int answer, base, last;

     printf( "Please enter an integer number greater than zero.\n" );
     printf( ">> " );
     fflush( stdout );
     if ( scanf( "%llu", &base ) != 1 )
     {
          printf( "\nSorry, that is not a valid entry.\n\n" );
          return 1;
     }
     printf( "\n" );
     answer = last = 1;

     /*

          Here we keep track of the previous or last number as
          well so we can detect when the data type has overflowed.

     */

     for( count = 1; count <= 1000 && answer >= last; count++ )
     {
          last = answer;
          answer = answer * base;

#ifdef DEBUG

printf( "count: %4d, answer: %llu, last: %llu\n", count, answer, last );

#endif

     }
     count--;

#ifdef DEBUG

     printf( "\n" );

#endif

     if ( answer < last )
     {
          count--;
          printf( "\
The variable overflowed its capacity after %d cycles.\n\n",
                  count );
          printf( "%llu ^ %d = %llu.\n\n", base, count, last );
     }
     else
     {
          printf( "%llu ^ %d = %llu.\n\n", base, count, answer );
     }

     return 0;
}

/* EOF multiple.c */
