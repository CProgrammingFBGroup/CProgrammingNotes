/*
     duration.c

     This is an open source library file, not a complete program.

     Receives a value in seconds and returns a string indicating
     weeks, days, hours, minutes, and seconds.  Uses 'use_short'
     to abbreviate the words to a single letter.

     Returns the text string, without a newline or a carriage
     return, or NULL if an error occured.

     Written by Matthew Campbell on Thursday January 17, 2019.

*/

#ifndef _DURATION_C
#define _DURATION_C

/* snprintf(3) needs this. */

#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200112L
#endif

#include <errno.h>
#include <stdio.h>

#ifndef EINVAL
#define EINVAL 22
#endif

/*

    'str' includes enough space to allow
    adding a period and a crlf combination.

*/

char *duration( unsigned long int seconds, int use_short )
{
     static char str[ 52 ];
     long int pos, ret;
     unsigned long int days, hours, len, minutes, weeks;

     if ( use_short != 1 && use_short != 0 )
     {
          errno = EINVAL;  /* Invalid value for 'use_short'. */
          return NULL;
     }

     len = 50;  /* This is the length of 'str' - 2.    */
     pos = 0;   /* This is the offset into the string. */

     /* We must return something, even if 'seconds' is zero. */

     if ( seconds == 0 )
     {
          if ( use_short == 1 )
          {
               errno = 0;
               ret = snprintf( &str[ pos ], len, "0s" );
               if ( ret == ( -1 ) )
               {
                    return NULL;  /* Something went wrong. */
               }
          }
          else
          {
               errno = 0;
               ret = snprintf( &str[ pos ], len, "0 seconds" );
               if ( ret == ( -1 ) )
               {
                    return NULL;  /* Something went wrong. */
               }
          }
          return str;
     }

     /* Find the number of weeks. */

     weeks = seconds / 604800;  /* 60 * 60 * 24 * 7 */
     seconds -= weeks * 604800;

     /* Find the number of days. */

     days = seconds / 86400;  /* 60 * 60 * 24 */
     seconds -= days * 86400;

     /* Find the number of hours. */

     hours = seconds / 3600;  /* 60 * 60 */
     seconds -= hours * 3600;

     /* Find the number of minutes. */

     minutes = seconds / 60;
     seconds -= minutes * 60;

     /* Now 'seconds' only holds the number of seconds. */

     if ( use_short == 1 )
     {
          /* Include the number of weeks, if any. */

          if ( weeks > 0 )
          {
               errno = 0;
               ret = snprintf( &str[ pos ], len, "%luw ", weeks );
               if ( ret == ( -1 ) )
               {
                    return NULL;  /* Something went wrong. */
               }
               pos += ret;
          }

          /* Include the number of days, if any. */

          if ( days > 0 )
          {
               errno = 0;
               ret = snprintf( &str[ pos ], len, "%lud ", days );
               if ( ret == ( -1 ) )
               {
                    return NULL;  /* Something went wrong. */
               }
               pos += ret;
          }

          /* Include the number of hours, if any. */

          if ( hours > 0 )
          {
               errno = 0;
               ret = snprintf( &str[ pos ], len, "%luh ", hours );
               if ( ret == ( -1 ) )
               {
                    return NULL;  /* Something went wrong. */
               }
               pos += ret;
          }

          /* Include the number of minutes, if any. */

          if ( minutes > 0 )
          {
               errno = 0;
               ret = snprintf( &str[ pos ], len, "%lum ", minutes );
               if ( ret == ( -1 ) )
               {
                    return NULL;  /* Something went wrong. */
               }
               pos += ret;
          }

          /* Include the number of seconds, if any. */

          if ( seconds > 0 )
          {
               errno = 0;
               ret = snprintf( &str[ pos ], len, "%lus ", seconds );
               if ( ret == ( -1 ) )
               {
                    return NULL;  /* Something went wrong. */
               }
               pos += ret;
          }
     }
     else  /* use_short == 0 */
     {
          /* Include the number of weeks, if any. */

          if ( weeks > 1 )
          {
               errno = 0;
               ret = snprintf( &str[ pos ], len, "%lu weeks ", weeks );
               if ( ret == ( -1 ) )
               {
                    return NULL;  /* Something went wrong. */
               }
               pos += ret;
          }
          else if ( weeks == 1 )
          {
               errno = 0;
               ret = snprintf( &str[ pos ], len, "1 week " );
               if ( ret == ( -1 ) )
               {
                    return NULL;  /* Something went wrong. */
               }
               pos += ret;
          }

          /* Include the number of days, if any. */

          if ( days > 1 )
          {
               errno = 0;
               ret = snprintf( &str[ pos ], len, "%lu days ", days );
               if ( ret == ( -1 ) )
               {
                    return NULL;  /* Something went wrong. */
               }
               pos += ret;
          }
          else if ( days == 1 )
          {
               errno = 0;
               ret = snprintf( &str[ pos ], len, "1 day " );
               if ( ret == ( -1 ) )
               {
                    return NULL;  /* Something went wrong. */
               }
               pos += ret;
          }

          /* Include the number of hours, if any. */

          if ( hours > 1 )
          {
               errno = 0;
               ret = snprintf( &str[ pos ], len, "%lu hours ", hours );
               if ( ret == ( -1 ) )
               {
                    return NULL;  /* Something went wrong. */
               }
               pos += ret;
          }
          else if ( hours == 1 )
          {
               errno = 0;
               ret = snprintf( &str[ pos ], len, "1 hour " );
               if ( ret == ( -1 ) )
               {
                    return NULL;  /* Something went wrong. */
               }
               pos += ret;
          }

          /* Include the number of minutes, if any. */

          if ( minutes > 1 )
          {
               errno = 0;
               ret = snprintf( &str[ pos ], len, "%lu minutes ", minutes );
               if ( ret == ( -1 ) )
               {
                    return NULL;  /* Something went wrong. */
               }
               pos += ret;
          }
          else if ( minutes == 1 )
          {
               errno = 0;
               ret = snprintf( &str[ pos ], len, "1 minute " );
               if ( ret == ( -1 ) )
               {
                    return NULL;  /* Something went wrong. */
               }
               pos += ret;
          }

          /* Include the number of seconds, if any. */

          if ( seconds > 1 )
          {
               errno = 0;
               ret = snprintf( &str[ pos ], len, "%lu seconds ", seconds );
               if ( ret == ( -1 ) )
               {
                    return NULL;  /* Something went wrong. */
               }
               pos += ret;
          }
          else if ( seconds == 1 )
          {
               errno = 0;
               ret = snprintf( &str[ pos ], len, "1 second " );
               if ( ret == ( -1 ) )
               {
                    return NULL;  /* Something went wrong. */
               }
               pos += ret;
          }
     }    /* if ( use_short == 1 ) */

     /* Remove the space character on the end of the string. */

     pos--;
     str[ pos ] = 0;

     /* And we're done. */

     return str;
}

#endif  /* _DURATION_C */

/* EOF duration.c */
