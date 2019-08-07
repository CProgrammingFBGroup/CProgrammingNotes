/*

     does_file_exist.c

     This function determines if the file "name" exists.
     Returns 1 for yes, 0 for no, and -1 for an error condition.
     Written by Matthew Campbell.  (12-15-17)

*/

#ifndef _DOES_FILE_EXIST_C
#define _DOES_FILE_EXIST_C

#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

int does_file_exist( const char *name )
{
     int ret;
     struct stat status;

     if ( name == NULL || name[ 0 ] == 0 )
     {
          return ( -1 );  /* Invalid file name. */
     }
     ret = stat( name, &status );
     if ( ret != 0 )
     {
          if ( errno == ENOENT )
          {
               return 0;  /* No such file or directory. */
          }
          else
          {
               return ( -1 );  /* Some other error. */
          }
     }
     return 1;  /* File exists. */
}

#endif  /* _DOES_FILE_EXIST_C */

/* EOF does_file_exist.c */
