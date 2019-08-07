/*

     biggest_number.c
     Finds the biggest number in the list.
     Written by Matthew Campbell on March 6, 2018.

*/

#include <stdio.h>

int main( void )
{
     int a[ 10 ], i, k, j = 0;

     printf("Please enter 10 numbers:\n" );
     for( k = 0; k < 10; k++ )
     {
          scanf( "%d", &a[ k ] );
     }
     for( i = 1; i < 10; i++ )
     {
         if ( a[ j ] < a[ i ] )
         {
              j = i;
         }
     }
     printf( "Answer: %d.\n", a[ j ] );
     return 0;
}

/* EOF */
