/*

     create_file.c

     This function creates the file "name" if it does not already exist.
     Returns 0 for success, 1 for file already exists, and -1 for an error condition.
     Written by Matthew Campbell.  (12-15-17)

*/

#ifndef _CREATE_FILE_C
#define _CREATE_FILE_C

#include <stdio.h>

int does_file_exist( const char *name );

int create_file( const char *name )
{
     FILE *fp;
     int ret;

     if ( name == NULL || name[ 0 ] == 0 )
     {
          return ( -1 );  /* Invalid file name. */
     }
     ret = does_file_exist( name );
     if ( ret == ( -1 ) )
     {
          return ( -1 );  /* Error. */
     }
     else if ( ret == 1 )
     {
          return 1;  /* File already exists. */
     }
     fp = fopen( name, "w" );
     if ( fp == NULL )
     {
          return ( -1 );  /* Error. */
     }
     fclose( fp );
     fp = NULL;
     ret = does_file_exist( name );
     if ( ret != 1 )
     {
          return ( -1 );  /* Error. */
     }
     return 0;  /* Success. */
}

#endif  /* _CREATE_FILE_C */

/* EOF create_file.c */
