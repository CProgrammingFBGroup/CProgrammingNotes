/*

     enter_to_continue.c
     Allows a program to tell the user to press Enter to continue.
     Written by Matthew Campbell on Saturday November 3, 2018.

     Example usage:

     printf( "Press Enter to continue." );
     fflush( stdout );
     enter_to_continue();

*/

#ifndef _ENTER_TO_CONTINUE_C
#define _ENTER_TO_CONTINUE_C

#include <stdio.h>

void enter_to_continue( void )
{
     char buffer[ 3 ];

     fgets( buffer, 3, stdin );
     return;
}

#endif  /* _ENTER_TO_CONTINUE_C */

/* EOF enter_to_continue.c */
