/*

     display_stat_structure.c

     Written by Matthew Campbell.  (12-15-17)

     Updated on Wednesday September 26, 2018 to include mode_string().

*/

#ifndef _DISPLAY_STAT_STRUCTURE_C
#define _DISPLAY_STAT_STRUCTURE_C

#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

/*

     I copied these two macros from an
     include file that came with my compiler.

*/

#define MAJOR( x ) \
	( ( unsigned long int )( ( ( ( x ) >> 32 ) & 0xFFFFF000 ) | \
 ( ( ( x ) >> 8 ) & 0x00000FFF ) ) )

#define MINOR( x ) \
	( ( unsigned long int )( ( ( ( x ) >> 12 ) & 0xFFFFFF00 ) | ( ( x ) & \
 0x000000FF ) ) )

/* Function prototypes: */

      int   display_stat_structure( const struct stat *ptr );
      void  init_stat_structure( struct stat *ptr );
const char *mode_string( mode_t mode );

/* Function definitions: */

/*

     Displays the structure used by stat().
     Returns 0 for success or 1 if an error occurs.

*/

int display_stat_structure( const struct stat *ptr )
{
     time_t tmp;

     if ( ptr == NULL )
     {
          return 1;  /* Error. */
     }

     /* ID of device containing file: */

     printf( "    st_dev: %llu, major: %lu, minor: %lu\n",
             ptr->st_dev, MAJOR( ptr->st_dev ), MINOR( ptr->st_dev ) );

     /* File inode number: */

     printf( "    st_ino: %llu\n", ptr->st_ino );

     /* Protection bits: */

     printf( "   st_mode: 0%o, string: %s\n", ptr->st_mode,
             mode_string( ptr->st_mode ) );

     /* Number of hard links to this file: */

     printf( "  st_nlink: %u\n", ptr->st_nlink );

     /* UID of file owner: */

     printf( "    st_uid: %u\n", ptr->st_uid );

     /* GID of file owner: */

     printf( "    st_gid: %u\n", ptr->st_gid );

     /* Device id, if special file: */

     printf( "   st_rdev: %llu\n", ptr->st_rdev );

     /* File size in bytes: */

     printf( "   st_size: %llu\n", ptr->st_size );

     /* I/O block size: */

     printf( "st_blksize: %lu\n", ptr->st_blksize );

     /* Number of 512 byte blocks allocated: */

     printf( " st_blocks: %lld, (%llu bytes)\n",
             ptr->st_blocks, ( ptr->st_blocks * 512 ) );

     /* Last access time: */

     tmp = ptr->st_atime;
     printf( "  st_atime: %s", ctime( &tmp ) );

     /* Last modification time: */

     tmp = ptr->st_mtime;
     printf( "  st_mtime: %s", ctime( &tmp ) );

     /* Time of last status change: */

     tmp = ptr->st_ctime;
     printf( "  st_ctime: %s", ctime( &tmp ) );

     return 0;  /* Success. */
}

/*

     Initializes all the structure members in the structure used by stat().

*/

void init_stat_structure( struct stat *ptr )
{
     if ( ptr == NULL )
     {
          return;
     }
     ptr->st_dev = 0;
     ptr->st_ino = 0;
     ptr->st_mode = 0;
     ptr->st_nlink = 0;
     ptr->st_uid = 0;
     ptr->st_gid = 0;
     ptr->st_rdev = 0;
     ptr->st_size = 0;
     ptr->st_blksize = 0;
     ptr->st_blocks = 0;
     ptr->st_atime = 0;
     ptr->st_mtime = 0;
     ptr->st_ctime = 0;
     return;
}

/*

     Converts 'mode' to the string used by /bin/ls.

*/

const char *mode_string( mode_t mode )  /* mode_t is unsigned */
{
     static char mode_str[ 11 ];
     int bits;

     bits = mode / 010000;
     bits = bits * 010000;
     bits = mode - bits;

     /* File type */

     if ( S_ISBLK( mode ) )
     {
          mode_str[ 0 ] = 'b';
     }
     else if ( S_ISCHR( mode ) )
     {
          mode_str[ 0 ] = 'c';
     }
     else if ( S_ISDIR( mode ) )
     {
          mode_str[ 0 ] = 'd';
     }
     else if ( S_ISFIFO( mode ) )
     {
          mode_str[ 0 ] = 'p';
     }
     else if ( S_ISLNK( mode ) )
     {
          mode_str[ 0 ] = 'l';
     }
     else if ( S_ISREG( mode ) )
     {
          mode_str[ 0 ] = '-';
     }
     else if ( S_ISSOCK( mode ) )
     {
          mode_str[ 0 ] = 's';
     }
     else  /* Unknown */
     {
          mode_str[ 0 ] = '?';
     }

     /* Owner */

     if ( bits & 0400 )
     {
          mode_str[ 1 ] = 'r';
     }
     else
     {
          mode_str[ 1 ] = '-';
     }
     if ( bits & 0200 )
     {
          mode_str[ 2 ] = 'w';
     }
     else
     {
          mode_str[ 2 ] = '-';
     }
     if ( bits & 0100 )
     {
          if ( bits & 04000 )
          {
               mode_str[ 3 ] = 's';
          }
          else
          {
               mode_str[ 3 ] = 'x';
          }
     }
     else
     {
          if ( bits & 04000 )
          {
               mode_str[ 3 ] = 'S';
          }
          else
          {
               mode_str[ 3 ] = '-';
          }
     }

     /* Group */

     if ( bits & 040 )
     {
          mode_str[ 4 ] = 'r';
     }
     else
     {
          mode_str[ 4 ] = '-';
     }
     if ( bits & 020 )
     {
          mode_str[ 5 ] = 'w';
     }
     else
     {
          mode_str[ 5 ] = '-';
     }
     if ( bits & 010 )
     {
          if ( bits & 02000 )
          {
               mode_str[ 6 ] = 's';
          }
          else
          {
               mode_str[ 6 ] = 'x';
          }
     }
     else
     {
          if ( bits & 02000 )
          {
               mode_str[ 6 ] = 'S';
          }
          else
          {
               mode_str[ 6 ] = '-';
          }
     }

     /* World */

     if ( bits & 04 )
     {
          mode_str[ 7 ] = 'r';
     }
     else
     {
          mode_str[ 7 ] = '-';
     }
     if ( bits & 02 )
     {
          mode_str[ 8 ] = 'w';
     }
     else
     {
          mode_str[ 8 ] = '-';
     }
     if ( bits & 01 )
     {
          if ( bits & 01000 )
          {
               mode_str[ 9 ] = 't';
          }
          else
          {
               mode_str[ 9 ] = 'x';
          }
     }
     else
     {
          if ( bits & 01000 )
          {
               mode_str[ 9 ] = 'T';
          }
          else
          {
               mode_str[ 9 ] = '-';
          }
     }

     mode_str[ 10 ] = 0;
     return mode_str;
}

#endif  /* _DISPLAY_STAT_STRUCTURE_C */

/* EOF display_stat_structure.c */
