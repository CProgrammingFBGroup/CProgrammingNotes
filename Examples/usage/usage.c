/*

     usage.c
     Gathers usage information for a mounted file system.
     Written by Matthew Campbell.

*/

/* popen(3) and pclose(3) need this: */

#define _POSIX_C_SOURCE 2

/* Include what we need: */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main( int argc, char **argv )
{
     FILE *fp;
     char device[ 11 ], mount_point[ 1025 ], str[ 1061 ];
     int available, len, match, percentage, save_errno, total, used;

     /* Make sure we have something to look for. */

     if ( argc != 2 )
     {
          printf( "Usage: usage <device | mount point>\n" );
          return 1;
     }

     /* Make sure it will fit in the available space. */

     len = ( int )strlen( argv[ 1 ] );

     if ( len > 1024 )
     {
          printf( "Usage: The device or mount point string is too long.\n" );
          return 1;
     }

     if ( argv[ 1 ][ 0 ] != '/' )
     {
          printf( "Usage: The device or mount point string was not found.\n" );
          return 1;
     }

     /* Prepare the command string. */

     sprintf( str, "/usr/bin/df -hB4k | /usr/bin/grep \"%s\"", argv[ 1 ] );

     /* Go get the information from the system. */

     errno = 0;
     fp = popen( str, "r" );

     if ( fp == NULL )
     {
          save_errno = errno;
          printf( "Usage: Failed to open the command pipe.\n" );

          if ( save_errno != 0 )
          {
               printf( "Error: %s.\n", strerror( save_errno ) );
          }

          return 1;
     }

     match = 0;

     while( match != 1 )
     {
          errno = 0;

          if ( fgets( str, 1024, fp ) == NULL )
          {
               save_errno = errno;
               printf( "\
Usage: The device or mount point string was not found.\n" );

               if ( save_errno != 0 )
               {
                    printf( "Error: %s.\n", strerror( save_errno ) );
               }

               pclose( fp );
               fp = NULL;
               return 1;
          }

          if ( str[ 0 ] == 0 )
          {
               printf( "\
Usage: The device or mount point string was not found.\n" );
               return 1;
          }

          if ( sscanf( str, "%s %d %d %d %d%% %s", device, &total, &used,
                       &available, &percentage, mount_point ) != 6 )
          {
               printf( "Usage: Failed to process the response string.\n" );
               return 1;
          }

          /* Make sure it actually matches the requested string. */

          if ( strcmp( device, argv[ 1 ] ) == 0 ||
               strcmp( mount_point, argv[ 1 ] ) == 0 )
          {
               match = 1;
          }

     }    /* while( match != 1 ) */

     printf( "      Device string: %s\n", device );
     printf( "    Total 4k blocks: %d\n", total );
     printf( "     4k blocks used: %d\n", used );
     printf( "4k blocks available: %d\n", available );
     printf( "    Percentage used: %d%%\n", percentage );
     printf( "        Mount point: %s\n", mount_point );

     /* Close the command pipe. */

     errno = 0;

     if ( pclose( fp ) != 0 )
     {
          save_errno = errno;
          printf( "\
Usage: Something went wrong while trying to close the command pipe.\n" );

          if ( save_errno != 0 )
          {
               printf( "Error: %s.\n", strerror( save_errno ) );
          }

          fp = NULL;
          return 1;
     }

     fp = NULL;

     return 0;
}

/* EOF usage.c */
