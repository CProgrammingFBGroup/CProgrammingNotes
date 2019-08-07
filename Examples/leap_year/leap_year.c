/*

     leap_year.c

     This program finds out if a specific year is a leap year.

     The Gregorian calendar first officially
     started using leap years in 1752 AD.

     Written by Matthew Campbell on Thursday September 6, 2018.

*/

#include <stdio.h>
#include <stdlib.h>

int main( void )
{
     int year;

     printf( "Year number? >> " );
     fflush( stdout );
     if ( scanf( "%d", &year ) != 1 )
     {
          printf( "Sorry, that is not a valid entry.\n" );
          exit( EXIT_FAILURE );
     }
     if ( year == 0 )
     {
          printf( "Sorry, that is not a valid entry.\n" );
          exit( EXIT_FAILURE );
     }
     if ( year < 1752 )
     {
          printf( "Year %d is not a leap year.\n", year );
          exit( EXIT_SUCCESS );
     }
     if ( ( year % 100 ) == 0 )
     {
          if ( ( year % 400 ) == 0 )
          {
               printf( "Year %d is a leap year.\n", year );
          }
          else
          {
               printf( "Year %d is not a leap year.\n", year );
          }
     }
     else
     {
          if ( ( year % 4 ) == 0 )
          {
               printf( "Year %d is a leap year.\n", year );
          }
          else
          {
               printf( "Year %d is not a leap year.\n", year );
          }
     }
     exit( EXIT_SUCCESS );
}

/* EOF leap_year.c */
