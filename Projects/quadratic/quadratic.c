/*
  *  quadratic.c
  *  Solves quadratic equations using the quadratic formula.
  *  Written by Matthew Campbell for the C Programming group.  (12-11-17)
  *
  */

#include <math.h>
#include <stdio.h>

#undef DEBUG

int main( void )
{
     char ch;
     double tmp, x1, x2;
     int a, b, c, flag;

     ch = 0;
     a = b = c = flag = 0;
     tmp = x1 = x2 = 0.0;
     printf( "\n\
This program will attempt to solve a quadratic equation in the form of\n\n\
ax^2 + bx + c = 0\n\n\
using the quadradic formula.  Please enter your values for a, b, and c below.\n\n\
" );
     printf( "Value for a >> " );
     fflush( stdout );
     do
     {
          flag = scanf( "%d", &a );
          if ( !flag )
          {
               printf( "Sorry, that is not a valid entry.  Please try again.\n\n" );
               ch = ( char )fgetc( stdin );  /* Clear the newline. */
               printf( "Value for a >> " );
               fflush( stdout );
          }
     }   while( !flag );
     printf( "Value for b >> " );
     fflush( stdout );
     do
     {
          flag = scanf( "%d", &b );
          if ( !flag )
          {
               printf( "Sorry, that is not a valid entry.  Please try again.\n\n" );
               ch = ( char )fgetc( stdin );  /* Clear the newline. */
               printf( "Value for b >> " );
               fflush( stdout );
          }
     }   while( !flag );
     printf( "Value for c >> " );
     fflush( stdout );
     do
     {
          flag = scanf( "%d", &c );
          if ( !flag )
          {
               printf( "Sorry, that is not a valid entry.  Please try again.\n\n" );
               ch = ( char )fgetc( stdin );  /* Clear the newline. */
               printf( "Value for c >> " );
               fflush( stdout );
          }
     }   while( !flag );
     flag = 0;
     printf( "\n" );

#ifdef DEBUG
     printf( ":DEBUG: a: %d, b: %d, c: %d\n", a, b, c );
#endif

     if ( !a && !b )
     {
          printf( "This problem does not appear to have a solution.\n\n" );
          return 1;
     }
     tmp = ( b * b ) - ( 4 * a * c );

#ifdef DEBUG
     printf( ":DEBUG: tmp: %G\n", tmp );
#endif

     if ( tmp < 0.0 )
     {
          printf( "This problem does not appear to have any real roots.\n\n" );
          return 2;
     }
     flag = 2 * a;

#ifdef DEBUG
     printf( ":DEBUG: flag: %d\n", flag );
#endif

     /* I would have refused a zero value for a above, but that      */
     /* would not have satisfied the assignment's requirements. */

     if ( !flag )  /* Prevent divide by zero. */
     {
          printf( "Value for a times 2 cannot be zero.\n\n" );
          return 3;
     }
     tmp = sqrt( tmp );

#ifdef DEBUG
     printf( ":DEBUG: sqrt( tmp ): %G\n\n", tmp );
#endif

     x1 = ( ( -b ) + tmp ) / flag;
     x2 = ( ( -b ) - tmp ) / flag;
     flag = 0;
     tmp = 0.0;
     printf( "Root 1: %G\nRoot 2: %G\n\n", x1, x2 );
     x1 = x2 = 0.0;
     a = b = c = 0;
     return 0;
}

/* EOF quadratic.c */