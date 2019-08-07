/*

     scramble.c
     Uses exclusive or to scramble text strings.
     Written by Matthew Campbell on Saturday September 8, 2018.

*/

int read_stdin( char *buffer, const int length, const char *prompt );

int main( void )
{
     char ch, str[ 1024 ], cipher[ 1024 ];
     int pos1, pos2, size1, size2;

     if ( read_stdin( str, 1024, "Please enter some text. >> " ) != 0 )
     {
          printf( "There was a problem while reading input.\n" );
          if ( errno )
          {
               printf( "Error: %s.\n", strerror( errno ) );
          }
          return 1;
     }
     size1 = strlen( str );
     if ( size1 < 1 )
     {
          printf( "No data read.\n" );
          return 1;
     }
     if ( read_stdin( cipher, 1024,
                      "Please enter a password to use. >> " ) != 0 )
     {
          printf( "There was a problem while reading input.\n" );
          if ( errno )
          {
               printf( "Error: %s.\n", strerror( errno ) );
          }
          return 1;
     }
     size2 = strlen( cipher );
     if ( size2 < 1 )
     {
          printf( "No data read.\n" );
          return 1;
     }
     printf( "Using str: \"%s\" and password: \"%s\"\n", str, cipher );
     pos1 = pos2 = 0;
     while( pos1 < size1 )
     {
          ch = str[ pos1 ] ^ cipher[ pos2 ];
          if ( ch < 33 )
          {
               ch += 33;  /* Just to make things printable. */
          }
          str[ pos1 ] = ch;
          pos1++;
          pos2++;
          if ( pos2 == size2 )
          {
               pos2 = 0;
          }
     }

     /* This is not guaranteed to be printable to the screen. */

     printf( "New str: \"%s\"\n", str );
     return 0;
}

/* EOF scramble.c */
