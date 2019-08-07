/*

     flush_stdin.c

     Clears the stdin buffer when using unbuffered I/O (Not stdio.h).

     This will only work if stdin is a terminal (tty).
     It will not work if stdin has been redirected from a file.

     Returns 0 for success and 1 if an error occurred.

     Written by Matthew Campbell on Sunday September 2, 2018.

*/

#ifndef _FLUSH_STDIN_C
#define _FLUSH_STDIN_C

#include <unistd.h>
#include <termios.h>

int flush_stdin( void )
{
     char ch;
     int icanon_state, min_value, ret, time_value;
     ssize_t read_ret;  /* This is a 32 bit signed int on my tablet. */
     struct termios data;

     /* Get the terminal data. */

     ret = tcgetattr( 0, &data );  /* File number 0 is stdin. */
     if ( ret != 0 )
     {
          return 1;  /* That didn't work. */
     }

     if ( data.c_lflag & ICANON  )
     {
          /* Turn off icanon. */

          data.c_lflag = data.c_lflag - ICANON;

          /* Keep track of its initial value. */

          icanon_state = 1;
     }
     else
     {
          icanon_state = 0;
     }

     /* Keep track of the other initial values too. */

     min_value = data.c_cc[ VMIN ];
     time_value = data.c_cc[ VTIME ];

     /* Set to return right away if nothing is in the buffer. */

     data.c_cc[ VMIN ] = 0;
     data.c_cc[ VTIME ] = 0;

     /* Set the updated terminal data. */

     ret = tcsetattr( 0, TCSANOW, &data );
     if ( ret != 0 )
     {
          return 1;  /* It didn't work. */
     }

     /* Now we can read any data left in the stdin buffer. */

     do
     {
          read_ret = read( 0, &ch, 1 );
     }    while( read_ret > 0 );

     /* Put everything back the way it was. */

     data.c_cc[ VMIN ] = min_value;
     data.c_cc[ VTIME ] = time_value;
     if ( icanon_state == 1 )
     {
          data.c_lflag = data.c_lflag + ICANON;
     }

     /* Set the updated terminal data. */

     ret = tcsetattr( 0, TCSANOW, &data );
     if ( ret != 0 )
     {
          return 1;  /* It didn't work. */
     }

     /* We're done. */

     return 0;
}

#endif

/* EOF flush_stdin.c */
