/*

     sizes.c
     Shows the size of each specified data type.
     Written by Matthew Campbell on Monday November 5, 2018.

*/

#include <stdio.h>

int main( void )
{
     printf( "  sizeof( char ): %d\n",
             sizeof( char ) );
     printf( "sizeof( char * ): %d\n",
             sizeof( char * ) );

     printf( "  sizeof( short int ): %d\n",
             sizeof( short int ) );
     printf( "sizeof( short int * ): %d\n",
             sizeof( short int * ) );

     printf( "  sizeof( int ): %d\n",
             sizeof( int ) );
     printf( "sizeof( int * ): %d\n",
             sizeof( int * ) );

     printf( "  sizeof( long int ): %d\n",
             sizeof( long int ) );
     printf( "sizeof( long int * ): %d\n",
             sizeof( long int * ) );

     printf( "  sizeof( long ): %d\n",
             sizeof( long ) );
     printf( "sizeof( long * ): %d\n",
             sizeof( long * ) );

     printf( "  sizeof( long long int ): %d\n",
             sizeof( long long int ) );
     printf( "sizeof( long long int * ): %d\n",
             sizeof( long long int * ) );

     printf( "  sizeof( float ): %d\n",
             sizeof( float ) );
     printf( "sizeof( float * ): %d\n",
             sizeof( float * ) );

     printf( "  sizeof( double ): %d\n",
             sizeof( double ) );
     printf( "sizeof( double * ): %d\n",
             sizeof( double * ) );

     printf( "  sizeof( long double ): %d\n",
             sizeof( long double ) );
     printf( "sizeof( long double * ): %d\n",
             sizeof( long double * ) );

     return 0;
}

/* EOF */


