/*

     number_pyramid.c
     Writes a pyramid with numbers.
     Written my Matthew Campbell on Sunday November 18, 2018.

*/

#include <stdio.h>

int main( void )
{
     int limit, limit_switch, num, pos1, pos2, rows, start_here;

     /* Print the prompt. */

     printf( "How many rows do you want? >> " );

     /* Use fflush() because the prompt didn't end with '\n'. */

     fflush( stdout );

     /* Find out how many rows to use. */

     if ( scanf( "%d", &rows ) != 1 )
     {
          printf( "\nThat is not a valid number.\n\n" );
          return 1;
     }

     /* Make sure we received a valid value. */

     if ( rows < 1 )
     {
          printf( "\nYou must use at least one row.\n\n" );
          return 1;
     }

     /* We can only use up to 9 rows if we use a single digit. */

     if ( rows > 9 )
     {
          printf( "\nYou must not use more than 9 rows.\n\n" );
          return 1;
     }

     /* Add a blank line after the prompt and the answer. */

     printf( "\n" );

     /* Initialize some variables. */

     limit = 1;
     num = 1;
     start_here = rows;

     /* The first loop controls the number of rows. */

     for( pos1 = 0; pos1 < rows; pos1++ )
     {
          /* The second loop controls the number of columns. */

          limit_switch = 0;
          for( pos2 = 1; limit_switch == 0 ||
               ( limit_switch == 1 && num > 0 ); pos2++ )
          {
               if ( pos2 < start_here )
               {
                    printf( "%c", 32 );  /* Space */
               }
               else
               {
                    printf( "%d", num );

          /* If we've reached our limit for this row, count backwards. */

                    if ( num == limit )
                    {
                         limit_switch = 1;
                    }
                    if ( limit_switch == 0 )
                    {
                         num++;
                    }
                    else
                    {
                         num--;
                    }
               }
          }

          /* Start the next row. */

          printf( "\n" );

          /* Use one number higher for each row. */

          limit++;

          /* Start counting at 1 again. */

          num = 1;

          /* Start printing numbers one column earlier. */

          start_here--;
     }

     /* Print an extra blank line and we're done. */

     printf( "\n" );
     return 0;
}

/* EOF number_pyramid.c */
