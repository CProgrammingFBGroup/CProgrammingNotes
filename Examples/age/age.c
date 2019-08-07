/*

     age.c
     Determines an age in years when supplied with a birthday.
     Written by Matthew Campbell on Wednesday January 2, 2019.

*/

#include <time.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Define DEBUG to get debugging output. */

#undef DEBUG

int main( void )
{
     char week_days[ 7 ][ 10 ] = { "Sunday", "Monday", "Tuesday",
                                   "Wednesday", "Thursday", "Friday",
                                   "Saturday" };
     int day, days, month, save_errno, this_year, year, years;
     struct tm birthday, *current;
     time_t birthday_time, current_time;

     /* Fetch the current time from the system clock. */

     current_time = time( NULL );
     errno = 0;
     current = localtime( &current_time );
     if ( current == NULL )
     {
          save_errno = errno;
          printf( "\
Sorry, something went wrong when attempting to determine the current time.\
\n" );
          if ( save_errno != 0 )
          {
               printf( "Error: %s.\n", strerror( save_errno ) );
          }
          printf( "\n" );
          exit( EXIT_FAILURE );
     }

     /* Gather input. */

     printf( "Please enter the birthday as MM DD YYYY >> " );
     fflush( stdout );
     if ( scanf( "%d %d %d", &month, &day, &year ) != 3 )
     {
          printf( "\nSorry that is not a valid entty.\n\n" );
          exit( EXIT_FAILURE );
     }
     printf( "\n" );

     /* Make sure we have been given valid values. */

     if ( year < 1900 )
     {
          printf( "The person's birthday is before the year 1900?\n" );
          printf( "Yeah sure.  Are they a vampire?\n\n" );
          exit( EXIT_FAILURE );
     }
     
     if ( month < 1 || month > 12 )
     {
          printf( "You have entered an invalid month: %d.\n\n", month );
          exit( EXIT_FAILURE );
     }

     if ( day < 1 || day > 31 )
     {
          printf( "You have entered an invalid day: %d.\n\n", day );
          exit( EXIT_FAILURE );
     }

     if ( month == 2 )
     {
          /* Check to see of it's a leap year. */

          if ( ( year % 100 ) == 0 )
          {
               if ( ( year % 400 ) == 0 )
               {
                    if ( day > 29 )
                    {
                         printf( "\
You have entered an invalid day: %d.\n\n", day );
                         exit( EXIT_FAILURE );
                    }
               }
               else  /* Not a leap year. */
               {
                    if ( day > 28 )
                    {
                         printf( "\
You have entered an invalid day: %d.\n\n", day );
                         exit( EXIT_FAILURE );
                    }
               }
          }
          else if ( ( year % 4 ) == 0 )
          {
               if ( day > 29 )
               {
                    printf( "You have entered an invalid day: %d.\n\n",
                            day );
                    exit( EXIT_FAILURE );
               }
          }
          else  /* Not a leap year. */
          {
               if ( day > 28 )
               {
                    printf( "You have entered an invalid day: %d.\n\n",
                            day );
                    exit( EXIT_FAILURE );
               }
          }
     }
     else if ( month == 4 || month == 6 || month == 9 || month == 11 )
     {
          if ( day > 30 )
          {
               printf( "You have entered an invalid day: %d.\n\n",
                       day );
               exit( EXIT_FAILURE );
          }
     }

     /* Put the birthday information into the structure. */

     birthday.tm_sec = 0;
     birthday.tm_min = 0;
     birthday.tm_hour = 0;
     birthday.tm_mday = day;
     birthday.tm_mon = month - 1;
     birthday.tm_year = year - 1900;
     birthday.tm_wday = 0;
     birthday.tm_yday = 0;
     birthday.tm_isdst = 0;

     /* Get the reference value in seconds for the birthday. */

     errno = 0;
     birthday_time = mktime( &birthday );
     if ( birthday_time == ( -1 ) )
     {
          save_errno = errno;
          printf( "\
Sorry, something went wrong when converting the birthday to seconds.\n" );
          if ( save_errno != 0 )
          {
               printf( "Error: %s.\n", strerror( save_errno ) );
          }
          printf( "\n" );
          exit( EXIT_FAILURE );
     }

     /* Make sure time travel wasn't involved. */

     if ( birthday_time > current_time )
     {
          printf( "\
They don't have an age if they haven't been born yet.\n\n" );
          exit( EXIT_FAILURE );
     }

     /* At this point we know the birthday is valid. */

     /* Has the birthday already passed during this year? */

     if ( current->tm_mon >= birthday.tm_mon &&
          current->tm_mday >= birthday.tm_mday )
     {
          this_year = 1;
     }
     else
     {
          this_year = 0;
     }

     years = current->tm_year - birthday.tm_year;
     if ( this_year == 0 )
     {
          years--;
     }

     days = ( current_time - birthday_time ) / 86400;

     /* Print the results. */

     if ( years == 1 )
     {
          printf( "Age: 1 year old.\n\n" );
     }
     else
     {
          printf( "Age: %d years old.\n\n", years );
     }

     printf( "In case you're curious, the age in days is " );
     if ( days == 1 )
     {
          printf( "1 day" );
     }
     else
     {
          printf( "%d days", days );
     }
     printf( " old.\n\n" );

     if ( birthday.tm_wday >= 0 && birthday.tm_wday <= 6 )
     {
          printf( "That birthday is on a %s.\n\n",
                  week_days[ birthday.tm_wday ] );
     }

     /* Print debugging output if requested. */

#ifdef DEBUG

     printf( "current_time: %lu\n\n", current_time );
     printf( "  tm_sec: %d\n", current->tm_sec );
     printf( "  tm_min: %d\n", current->tm_min );
     printf( " tm_hour: %d\n", current->tm_hour );
     printf( " tm_mday: %d\n", current->tm_mday );
     printf( "  tm_mon: %d\n", current->tm_mon );
     printf( " tm_year: %d\n", current->tm_year );
     printf( " tm_wday: %d\n", current->tm_wday );
     printf( " tm_yday: %d\n", current->tm_yday );
     printf( "tm_isdst: %d\n", current->tm_isdst );
     printf( "\n" );

     printf( "birthday_time: %lu\n\n", birthday_time );
     printf( "  tm_sec: %d\n", birthday.tm_sec );
     printf( "  tm_min: %d\n", birthday.tm_min );
     printf( " tm_hour: %d\n", birthday.tm_hour );
     printf( " tm_mday: %d\n", birthday.tm_mday );
     printf( "  tm_mon: %d\n", birthday.tm_mon );
     printf( " tm_year: %d\n", birthday.tm_year );
     printf( " tm_wday: %d\n", birthday.tm_wday );
     printf( " tm_yday: %d\n", birthday.tm_yday );
     printf( "tm_isdst: %d\n", birthday.tm_isdst );
     printf( "\n" );

#endif

     /* And we're done. */

     return 0;
}

/* EOF age.c */
