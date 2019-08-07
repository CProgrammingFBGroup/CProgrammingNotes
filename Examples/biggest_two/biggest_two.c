/*

     biggest_two.c
     Finds the biggest two numbers.
     Written by Matthew Campbell on Saturday November 3, 2018.

*/

#include <stdio.h>

int main( void )
{
     int a, b, c, d, e, flag, max, max2;

     a = -5;
     b = 4;
     c = 7;
     d = -2;
     e = 8;

     max = a;
     flag = 0;

     if ( max < b )
     {
          flag = 1;
          max2 = max;
          max = b;
     }
     if ( max < c )
     {
          flag = 1;
          max2 = max;
          max = c;
     }
     if ( max < d )
     {
          flag = 1;
          max2 = max;
          max = d;
     }
     if ( max < e )
     {
          flag = 1;
          max2 = max;
          max = e;
     }

     if ( !flag )  /* max2 was never set. */
     {
          if ( max > a )
          {
               max2 = a;
          }
          if ( max > b && max2 < b )
          {
               max2 = b;
          }
          if ( max > c && max2 < c )
          {
               max2 = c;
          }
          if ( max > d && max2 < d )
          {
               max2 = d;
          }
          if ( max > e && max2 < e )
          {
               max2 = e;
          }
     }

     printf( "a: %d, b: %d, c: %d, d: %d, e: %d, max: %d, max2: %d\n",
             a, b, c, d, e, max, max2 );
     return 0;
}

/* EOF */
