/*

     files.c

     Tests file access for apps.

     Written by Matthew Campbell on Thursday September 27, 2018.

     It took an extra six hours into the next day to finish this.

     Some of the functions that say they work
     with files can also work with directories.

*/

/* Define these so mknod(2) works. */

#define _BSD_SOURCE
#define _X_OPEN_SOURCE
#define _X_OPEN_SOURCE_EXTENDED

/* Gee, did we include enough include files? */

#include <time.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/sysmacros.h>

#define MAJOR_VERSION 0
#define MINOR_VERSION 1
#define PATCH_VERSION 1
#define UPDATE_VERSION 2

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

void print_main_menu( char *cwd );
void change_directory( char *cwd );
void show_files( char *cwd );
void stat_file( char *cwd );
void change_protection( char *cwd );
void create_new_file( char *cwd );
void read_from_file( char *cwd );
void write_to_file( char *cwd );
void rename_file( char *cwd );
void remove_file( char *cwd );
void create_directory( char *cwd );
void remove_directory( char *cwd );
int  look_for_slash( char *str );

/* These are external functions in other files: */

       int  create_file( const char *name );
       int  display_stat_structure( const struct stat *ptr );
       int  does_file_exist( const char *name );
       void init_stat_structure( struct stat *ptr );
const char *mode_string( mode_t mode );
       int  read_stdin( char *buffer, const int length, const char *prompt,
                        const int reprompt );

/* Function definitions: */

int main( void )
{
     char buffer[ 4 ], cwd[ 1025 ];
     int exit_loop, option, ret, save_errno;

     /* Intialize everything. */

     exit_loop = option = ret = save_errno = 0;

     printf( "Files version %d.%d.%d\n\n", MAJOR_VERSION, MINOR_VERSION,
             PATCH_VERSION );

     getcwd( cwd, 1025 );
     print_main_menu( cwd );

     do
     {
          getcwd( cwd, 1025 );
          ret = read_stdin( buffer, 4, "Main menu >> ", 1 );
          printf( "\n" );
          if ( ret != 0 )
          {
               save_errno = errno;
               printf( "\
Sorry, something went wrong.  Please try again.\n\n" );
               if ( save_errno != 0 )
               {
                    printf( "Error: %s.\n", strerror( save_errno ) );
               }
          }
          else
          {
               if ( sscanf( buffer, "%d", &option ) != 1 )
               {
                    printf( "\
Sorry, that is not valid input.  Please try again.\n\n" );
               }
               else
               {
                    if ( option < 0 || option > 12 )
                    {
                         printf( "\
Sorry, that is not a valid option.  Please try again.\n\n" );
                    }
                    else
                    {
                         switch( option )
                         {
                              case  0 : print_main_menu( cwd );
                                        break;
                              case  1 : change_directory( cwd );
                                        break;
                              case  2 : show_files( cwd );
                                        break;
                              case  3 : stat_file( cwd );
                                        break;
                              case  4 : change_protection( cwd );
                                        break;
                              case  5 : create_new_file( cwd );
                                        break;
                              case  6 : read_from_file( cwd );
                                        break;
                              case  7 : write_to_file( cwd );
                                        break;
                              case  8 : rename_file( cwd );
                                        break;
                              case  9 : remove_file( cwd );
                                        break;
                              case 10 : create_directory( cwd );
                                        break;
                              case 11 : remove_directory( cwd );
                                        break;
                              case 12 : printf( "Exiting.\n\n" );
                                        exit_loop = 1;
                                        break;
                              default : printf( "\
Something went wrong.  The default case was reached.\n\n" );
                                        break;
                         }    /* switch( option ) */
                    }    /* if ( option < 0 || option > 12 ) */
               }    /* if ( sscanf( buffer, "%d", &option ) != 1 ) */
          }    /* if ( ret != 0 ) */
     }    while( exit_loop == 0 );
     return 0;
}

/* This function prints the main menu. */

void print_main_menu( char *cwd )
{
     if ( cwd == NULL )
     {
          printf( "The current directory is undefined.\n\n" );
     }
     else
     {
          printf( "Current directory: %s\n\n", cwd );
     }

     printf( "\
 0) Print this menu.\n\
 1) Change to another directory.\n\
 2) Show the files in the current directory.\n\
 3) Stat a file in the current directory.\n\
 4) Change the protection bits for a file in the current directory.\n\
 5) Create a file in the current directory.\n\
 6) Read from a file in the current directory.\n\
 7) Write to a file in the current directory.\n\
 8) Rename a file in the current directory.\n\
 9) Remove a file in the current directory.\n\
10) Create a directory in the current directory.\n\
11) Remove a directory from the current directory.\n\
12) Exit.\n\n" );

     return;
}

/* This function attempts to chsnge the current working directory. */

void change_directory( char *cwd )
{
     char buffer[ 1026 ];
     int ret, save_errno;

     if ( cwd == NULL )
     {
          printf( "The current directory is undefined.\n\n" );
     }
     else
     {
          printf( "Current directory: %s\n\n", cwd );
     }

     printf( "What directory would you like to change to?\n" );
     ret = read_stdin( buffer, 1026, ">> ", 1 );
     save_errno = errno;
     printf( "\n" );
     if ( ret != 0 )
     {
          printf( "Sorry, something went wrong while reading.\n" );
          if ( save_errno != 0 )
          {
               printf( "Error: %s.\n", strerror( save_errno ) );
          }
          printf( "\n" );
          return;
     }

     ret = look_for_slash( buffer );
     if ( ret == ( -1 ) )
     {
          printf( "\
Sorry, something went wrong while processing your string.\n\n" );
          return;
     }

     if ( ret != 0 )
     {
          printf( "\
You must not include any forward slash \
characters '/' in your string.\n\n" );
          return;
     }

     /* Change to the new directory. */

     ret = chdir( buffer );
     if ( ret != 0 )
     {
          save_errno = errno;
          printf( "Failed to change to the new directory.\n" );
          if ( save_errno != 0 )
          {
               printf( "Error: %s.\n", strerror( save_errno ) );
          }
          printf( "\n" );
     }
     else
     {
          /* Changing to the same directory still counts as successful. */

          printf( "Successfully changed to the new directory.\n\n" );
     }
     return;
}

/* This function lists the files in the current directory. */

void show_files( char *cwd )
{
     DIR *dp;
     char *str;
     int exit_loop, first, ret, save_errno;
     struct dirent *dir;
     struct stat stats;

     if ( cwd == NULL )
     {
          printf( "The current directory is undefined.\n\n" );
          return;
     }
     else
     {
          printf( "Current directory: %s\n\n", cwd );
     }

     errno = 0;
     dp = opendir( "." );  /* Open the directory for reading. */

     if ( dp == NULL )
     {
          save_errno = errno;
          printf( "Failed to open the current directory.\n" );
          if ( save_errno != 0 )
          {
               printf( "Error: %s.\n", strerror( save_errno ) );
          }
          printf( "\n" );
          return;
     }

     first = 1;
     exit_loop = 0;
     do
     {
          errno = 0;
          dir = readdir( dp );  /* Read a directory entry. */
          if ( dir == NULL )
          {
               save_errno = errno;
               if ( save_errno != 0 )
               {
                    printf( "Error while reading directory: %s.\n",
                             strerror( save_errno ) );
                    errno = 0;
                    if ( closedir( dp ) != 0 )
                    {
                         save_errno = errno;
                         printf( "Failed to close the directory.\n" );
                         if ( save_errno != 0 )
                         {
                              printf( "Error: %s.\n",
                                      strerror( save_errno ) );
                         }
                    }
                    printf( "\n" );
                    dp = NULL;
                    return;
               }
               else
               {
                    if ( first == 1 )
                    {
                         printf( "Directory is empty.\n\n" );
                    }
                    exit_loop = 1;
               }
          }
          if ( dir != NULL )
          {
               init_stat_structure( &stats );
               errno = 0;
               ret = stat( dir->d_name, &stats );
               if ( ret != 0 )
               {
                    save_errno = errno;
                    printf( "Failed to stat the file: %s\n", dir->d_name );
                    if ( save_errno != 0 )
                    {
                         printf( "Error: %s.\n", strerror( save_errno ) );
                    }
               }
               else
               {
                    str = ctime( &stats.st_mtime );
                    str[ 24 ] = 0; /* Remove the newline. */

                    if ( S_ISBLK( stats.st_mode ) ||
                         S_ISCHR( stats.st_mode ) )
                    {
                         printf( "%s %5u %5u %4lu %5lu %s %s\n",
                                 mode_string( stats.st_mode ),
                                 stats.st_uid, stats.st_gid,
                                 MAJOR( stats.st_dev ),
                                 MINOR( stats.st_dev ),
                                 str, dir->d_name );
                    }
                    else
                    {
                         /*

                              stats.st_size was being promoted to an
                              unsigned long long int so the cast was
                              necessary to prevent this.

                         */

                         printf( "%s %5u %5u %10u %s %s\n",
                                 mode_string( stats.st_mode ),
                                 stats.st_uid, stats.st_gid,
                                 ( size_t )stats.st_size, str,
                                 dir->d_name );

                    }    /* if ( S_ISBLK() || S_ISCHR() ) */
               }    /* if ( ret != 0 ) */
          }    /* if ( dir != NULL ) */
          first = 0;
     }    while( exit_loop == 0 );
     printf( "\n" );

     errno = 0;
     if ( closedir( dp ) != 0 )
     {
          save_errno = errno;
          printf( "Failed to close the directory.\n" );
          if ( save_errno != 0 )
          {
               printf( "Error: %s.\n", strerror( save_errno ) );
          }
          printf( "\n" );
     }
     dp = NULL;
     return;
}

/* This function attempts to stat a file in the current directory. */

void stat_file( char *cwd )
{
     char buffer[ 1026 ];
     int ret, save_errno;
     struct stat stats;

     if ( cwd == NULL )
     {
          printf( "The current directory is undefined.\n\n" );
          return;
     }
     else
     {
          printf( "Current directory: %s\n\n", cwd );
     }

     printf( "What file would you like to stat?\n" );
     ret = read_stdin( buffer, 1026, ">> ", 1 );
     save_errno = errno;
     printf( "\n" );
     if ( ret != 0 )
     {
          printf( "Sorry, something went wrong while reading.\n" );
          if ( save_errno != 0 )
          {
               printf( "Error: %s.\n", strerror( save_errno ) );
          }
          printf( "\n" );
          return;
     }

     ret = look_for_slash( buffer );
     if ( ret == ( -1 ) )
     {
          printf( "\
Sorry, something went wrong while processing your string.\n\n" );
          return;
     }

     if ( ret != 0 )
     {
          printf( "\
You must not include any forward slash \
characters '/' in your string.\n\n" );
          return;
     }

     init_stat_structure( &stats );
     errno = 0;
     ret = stat( buffer, &stats );
     if ( ret != 0 )
     {
          save_errno = errno;
          printf( "Failed to stat the file: %s\n", buffer );
          if ( save_errno != 0 )
          {
               printf( "Error: %s.\n", strerror( save_errno ) );
          }
          printf( "\n" );
          return;
     }

     printf( "Showing stat information for the file: %s\n\n", buffer );
     display_stat_structure( &stats );
     printf( "\n" );
     return;
}

/* This function attempts to change the protection bits for a file. */

void change_protection( char *cwd )
{
     char buffer[ 1026 ], str[ 7 ];
     int ret, save_errno;
     mode_t old, new;
     struct stat stats;

     if ( cwd == NULL )
     {
          printf( "The current directory is undefined.\n\n" );
          return;
     }
     else
     {
          printf( "Current directory: %s\n\n", cwd );
     }

     printf( "\
What file would you like to change the protection bits for?\n" );
     ret = read_stdin( buffer, 1026, ">> ", 1 );
     save_errno = errno;
     printf( "\n" );
     if ( ret != 0 )
     {
          printf( "Sorry, something went wrong while reading.\n" );
          if ( save_errno != 0 )
          {
               printf( "Error: %s.\n", strerror( save_errno ) );
          }
          printf( "\n" );
          return;
     }

     ret = look_for_slash( buffer );
     if ( ret == ( -1 ) )
     {
          printf( "\
Sorry, something went wrong while processing your string.\n\n" );
          return;
     }

     if ( ret != 0 )
     {
          printf( "\
You must not include any forward slash \
characters '/' in your string.\n\n" );
          return;
     }

     init_stat_structure( &stats );
     errno = 0;
     ret = stat( buffer, &stats );
     if ( ret != 0 )
     {
          save_errno = errno;
          printf( "Failed to stat the file: %s\n", buffer );
          if ( save_errno != 0 )
          {
               printf( "Error: %s.\n", strerror( save_errno ) );
          }
          printf( "\n" );
          return;
     }

     /* Remove the file type bits. */

     old = stats.st_mode & 07777;
     printf( "The protection bits for the file: %s\n", buffer );
     printf( "are currently set to: 0%o\n\n", old );
     printf( "What would you like to try to change them to?\n" );
     ret = read_stdin( str, 7, ">> ", 1 );
     save_errno = errno;
     printf( "\n" );
     if ( ret != 0 )
     {
          printf( "Sorry, something went wrong while reading.\n" );
          if ( save_errno != 0 )
          {
               printf( "Error: %s.\n", strerror( save_errno ) );
          }
          printf( "\n" );
          return;
     }
     if ( sscanf( str, "%ho", &new ) != 1 )
     {
          printf( "Sorry, that is not valid input.\n\n" );
          return;
     }

     /* Add on the file type bits. */

     new = new + ( stats.st_mode - old );

     /* Change the protection mode bits. */

     errno = 0;
     ret = chmod( buffer, new );
     if ( ret != 0 )
     {
          save_errno = errno;
          printf( "\
Failed to change the protection bits for the file: %s\n", buffer );
          if ( save_errno != 0 )
          {
               printf( "Error: %s.\n", strerror( save_errno ) );
          }
          printf( "\n" );
          return;
     }

     /* Make sure it worked. */

     init_stat_structure( &stats );
     errno = 0;
     ret = stat( buffer, &stats );
     if ( ret != 0 )
     {
          save_errno = errno;
          printf( "Failed to stat the file: %s\n", buffer );
          if ( save_errno != 0 )
          {
               printf( "Error: %s.\n", strerror( save_errno ) );
          }
          printf( "\n" );
          return;
     }
     if ( stats.st_mode != new )
     {
          printf( "\
Failed to change the protection bits for the file: %s\n\n", buffer );
          return;
     }

     printf( "\
Successfully changed the protection bits for the file: %s\n\n",
             buffer );
     return;
}

/* This function attempts to create a new file in the current directory. */

void create_new_file( char *cwd )
{
     char buffer[ 12 ], name1[ 1026 ], name2[ 1026 ];
     dev_t device, major, minor;
     mode_t mode;
     int ret, save_errno, type;

     if ( cwd == NULL )
     {
          printf( "The current directory is undefined.\n\n" );
          return;
     }
     else
     {
          printf( "Current directory: %s\n\n", cwd );
     }

     printf( "What file would you like to create?\n" );
     ret = read_stdin( name1, 1026, ">> ", 1 );
     save_errno = errno;
     printf( "\n" );
     if ( ret != 0 )
     {
          printf( "Sorry, something went wrong while reading.\n" );
          if ( save_errno != 0 )
          {
               printf( "Error: %s.\n", strerror( save_errno ) );
          }
          printf( "\n" );
          return;
     }

     ret = look_for_slash( name1 );
     if ( ret == ( -1 ) )
     {
          printf( "\
Sorry, something went wrong while processing your string.\n\n" );
          return;
     }

     if ( ret != 0 )
     {
          printf( "\
You must not include any forward slash \
characters '/' in your string.\n\n" );
          return;
     }

     /* Find out if the file already exists. */

     errno = 0;
     ret = does_file_exist( name1 );
     if ( ret == ( -1 ) )
     {
          save_errno = errno;
          printf( "\
Sorry, something went wrong while looking for the file: %s\n", name1 );
          if ( save_errno != 0 )
          {
               printf( "Error: %s.\n", strerror( save_errno ) );
          }
          printf( "\n" );
          return;
     }

     if ( ret == 1 )
     {
          printf( "\
A file or directory with that name already \
exists in the current directory.\n\n" );
          return;
     }
     
     /* Find out what kind of file to create. */

     printf( "What type of file would you like to create?\n\n" );
     printf( "1) A block device file\n" );
     printf( "2) A character device file\n" );
     printf( "3) A hard link\n" );
     printf( "4) A named pipe (fifo) file\n" );
     printf( "5) A named socket\n" );
     printf( "6) A regular file\n" );
     printf( "7) A symbolic link\n\n" );
     ret = read_stdin( buffer, 3, ">> ", 1 );
     save_errno = errno;
     printf( "\n" );
     if ( ret != 0 )
     {
          printf( "Sorry, something went wrong while reading.\n" );
          if ( save_errno != 0 )
          {
               printf( "Error: %s.\n", strerror( save_errno ) );
          }
          printf( "\n" );
          return;
     }

     if ( sscanf( buffer, "%d", &type ) != 1 )
     {
          printf( "Sorry, that is not valid input.\n\n" );
          return;
     }

     if ( type < 1 || type > 7 )
     {
          printf( "Sorry, that is not a valid option.\n\n" );
          return;
     }

     if ( type == 1 )  /* Block device */
     {
          printf( "\
What is the major device number of the block device?\n" );
          ret = read_stdin( buffer, 12, ">> ", 1 );
          save_errno = errno;
          printf( "\n" );
          if ( ret != 0 )
          {
               printf( "Sorry, something went wrong while reading.\n" );
               if ( save_errno != 0 )
               {
                    printf( "Error: %s.\n", strerror( save_errno ) );
               }
               printf( "\n" );
               return;
          }
          if ( sscanf( buffer, "%u", &major ) != 1 )
          {
               printf( "Sorry, that is not valid input.\n\n" );
               return;
          }
          printf( "\
What is the minor device number of the block device?\n" );
          ret = read_stdin( buffer, 12, ">> ", 1 );
          save_errno = errno;
          printf( "\n" );
          if ( ret != 0 )
          {
               printf( "Sorry, something went wrong while reading.\n" );
               if ( save_errno != 0 )
               {
                    printf( "Error: %s.\n", strerror( save_errno ) );
               }
               printf( "\n" );
               return;
          }
          if ( sscanf( buffer, "%u", &minor ) != 1 )
          {
               printf( "Sorry, that is not valid input.\n\n" );
               return;
          }

          /* Make the block device file. */

          errno = 0;
          device = makedev( major, minor );
          mode = S_IFBLK | 0777;
          ret = mknod( name1, mode, device );
          if ( ret != 0 )
          {
               save_errno = errno;
               printf( "\
Failed to create the block device file: %s\n", name1 );
               if ( save_errno != 0 )
               {
                    printf( "Error: %s.\n", strerror( save_errno ) );
               }
               printf( "\n" );
               return;
          }

          /* Make sure it worked. */

          errno = 0;
          ret = does_file_exist( name1 );
          if ( ret == ( -1 ) )
          {
               save_errno = errno;
               printf( "\
Sorry, something went wrong while looking\n\
for the block device file: %s\n",
                       name1 );
               if ( save_errno != 0 )
               {
                    printf( "Error: %s.\n", strerror( save_errno ) );
               }
               printf( "\n" );
               return;
          }

          if ( ret == 0 )
          {
               printf( "\
Failed to find the block device file: %s\n\n", name1 );
          }
          else
          {
               printf( "\
Successfully created the block device file: %s\n\n", name1 );
          }
     }
     else if ( type == 2 )  /* Character device */
     {
          printf( "\
What is the major device number of the character device?\n" );
          ret = read_stdin( buffer, 12, ">> ", 1 );
          save_errno = errno;
          printf( "\n" );
          if ( ret != 0 )
          {
               printf( "Sorry, something went wrong while reading.\n" );
               if ( save_errno != 0 )
               {
                    printf( "Error: %s.\n", strerror( save_errno ) );
               }
               printf( "\n" );
               return;
          }
          if ( sscanf( buffer, "%u", &major ) != 1 )
          {
               printf( "Sorry, that is not valid input.\n\n" );
               return;
          }
          printf( "\
What is the minor device number of the character device?\n" );
          ret = read_stdin( buffer, 12, ">> ", 1 );
          save_errno = errno;
          printf( "\n" );
          if ( ret != 0 )
          {
               printf( "Sorry, something went wrong while reading.\n" );
               if ( save_errno != 0 )
               {
                    printf( "Error: %s.\n", strerror( save_errno ) );
               }
               printf( "\n" );
               return;
          }
          if ( sscanf( buffer, "%u", &minor ) != 1 )
          {
               printf( "Sorry, that is not valid input.\n\n" );
               return;
          }

          /* Make the character device file. */

          errno = 0;
          device = makedev( major, minor );
          mode = S_IFCHR | 0777;
          ret = mknod( name1, mode, device );
          if ( ret != 0 )
          {
               save_errno = errno;
               printf( "\
Failed to create the character device file: %s\n", name1 );
               if ( save_errno != 0 )
               {
                    printf( "Error: %s.\n", strerror( save_errno ) );
               }
               printf( "\n" );
               return;
          }

          /* Make sure it worked. */

          errno = 0;
          ret = does_file_exist( name1 );
          if ( ret == ( -1 ) )
          {
               save_errno = errno;
               printf( "\
Sorry, something went wrong while looking\n\
for the character device file: %s\n", name1 );
               if ( save_errno != 0 )
               {
                    printf( "Error: %s.\n", strerror( save_errno ) );
               }
               printf( "\n" );
               return;
          }

          if ( ret == 0 )
          {
               printf( "\
Failed to find the character device file: %s\n\n", name1 );
          }
          else
          {
               printf( "\
Successfully created the character device file: %s\n\n", name1 );
          }
     }
     else if ( type == 3 )  /* Hard link */
     {
          printf( "What file would you like to make a hard link to?\n" );
          ret = read_stdin( name2, 1026, ">> ", 1 );
          save_errno = errno;
          printf( "\n" );
          if ( ret != 0 )
          {
               printf( "Sorry, something went wrong while reading.\n" );
               if ( save_errno != 0 )
               {
                    printf( "Error: %s.\n", strerror( save_errno ) );
               }
               printf( "\n" );
               return;
          }

          ret = look_for_slash( name2 );
          if ( ret == ( -1 ) )
          {
               printf( "\
Sorry, something went wrong while processing your string.\n\n" );
               return;
          }

          if ( ret != 0 )
          {
               printf( "\
You must not include any forward slash \
characters '/' in your string.\n\n" );
               return;
          }

          /* Find out if the file exists. */

          errno = 0;
          ret = does_file_exist( name2 );
          if ( ret == ( -1 ) )
          {
               save_errno = errno;
               printf( "\
Sorry, something went wrong while looking\n\
for the file: %s\n", name1 );
               if ( save_errno != 0 )
               {
                    printf( "Error: %s.\n", strerror( save_errno ) );
               }
               printf( "\n" );
               return;
          }

          if ( ret == 0 )
          {
               printf( "\
The file \"%s\" was not found in the current directory.\n\n", name2 );
               return;
          }

          /* Make the hard link. */

          errno = 0;
          ret = link( name2, name1 );
          save_errno = errno;
          if ( ret != 0 )
          {
               printf( "\
Failed to create a hard link from \"%s\" to \"%s\"\n", name1, name2 );
               if ( save_errno != 0 )
               {
                    printf( "Error: %s.\n", strerror( save_errno ) );
               }
               printf( "\n" );
               return;
          }

          /* Make sure it worked. */

          errno = 0;
          ret = does_file_exist( name1 );
          if ( ret == ( -1 ) )
          {
               save_errno = errno;
               printf( "\
Sorry, something went wrong while looking\n\
for the hard link file: %s\n", name1 );
               if ( save_errno != 0 )
               {
                    printf( "Error: %s.\n", strerror( save_errno ) );
               }
               printf( "\n" );
               return;
          }

          if ( ret == 0 )
          {
               printf( "Failed to find the hard link named: %s\n\n",
                       name1 );
          }
          else
          {
               printf( "\
Successfully hard linked \"%s\" to \"%s\"\n\n", name1, name2 );
          }
     }
     else if ( type == 4 )  /* Named pipe (fifo) */
     {
          /* Make the named pipe (fifo) file. */

          errno = 0;
          mode = S_IFIFO | 0777;
          ret = mkfifo( name1, mode );
          if ( ret != 0 )
          {
               save_errno = errno;
               printf( "\
Failed to create the named pipe (fifo) file: %s\n", name1 );
               if ( save_errno != 0 )
               {
                    printf( "Error: %s.\n", strerror( save_errno ) );
               }
               printf( "\n" );
               return;
          }

          /* Make sure it worked. */

          errno = 0;
          ret = does_file_exist( name1 );
          if ( ret == ( -1 ) )
          {
               save_errno = errno;
               printf( "\
Sorry, something went wrong while looking\n\
for the named pipe (fifo) file: %s\n",
                       name1 );
               if ( save_errno != 0 )
               {
                    printf( "Error: %s.\n", strerror( save_errno ) );
               }
               printf( "\n" );
               return;
          }

          if ( ret == 0 )
          {
               printf( "\
Failed to find the named pipe (fifo) file: %s\n\n", name1 );
          }
          else
          {
               printf( "\
Successfully created the named pipe (fifo) file: %s\n\n", name1 );
          }
     }
     else if ( type == 5 )  /* Named socket */
     {
          /* Make the named socket file. */

          errno = 0;
          mode = S_IFSOCK | 0777;
          ret = mknod( name1, mode, 0 );
          if ( ret != 0 )
          {
               save_errno = errno;
               printf( "\
Failed to create the named socket file: %s\n", name1 );
               if ( save_errno != 0 )
               {
                    printf( "Error: %s.\n", strerror( save_errno ) );
               }
               printf( "\n" );
               return;
          }

          /* Make sure it worked. */

          errno = 0;
          ret = does_file_exist( name1 );
          if ( ret == ( -1 ) )
          {
               save_errno = errno;
               printf( "\
Sorry, something went wrong while looking\n\
for the named socket file: %s\n",
                       name1 );
               if ( save_errno != 0 )
               {
                    printf( "Error: %s.\n", strerror( save_errno ) );
               }
               printf( "\n" );
               return;
          }

          if ( ret == 0 )
          {
               printf( "\
Failed to find the named socket file: %s\n\n", name1 );
          }
          else
          {
               printf( "\
Successfully created the named socket file: %s\n\n", name1 );
          }
     }
     else if ( type == 6 )  /* Regular file */
     {
          ret = create_file( name1 );
          if ( ret != 0 )
          {
               save_errno = errno;
               printf( "Failed to create the regular file: %s\n", name1 );
               if ( save_errno != 0 )
               {
                    printf( "Error: %s.\n", strerror( save_errno ) );
               }
               printf( "\n" );
               return;
          }

          /* Make sure it worked. */

          errno = 0;
          ret = does_file_exist( name1 );
          if ( ret == ( -1 ) )
          {
               save_errno = errno;
               printf( "\
Sorry, something went wrong while looking\n\
for the regular file: %s\n", name1 );
               if ( save_errno != 0 )
               {
                    printf( "Error: %s.\n", strerror( save_errno ) );
               }
               printf( "\n" );
               return;
          }

          if ( ret == 0 )
          {
               printf( "Failed to find the regular file named: %s\n\n",
                       name1 );
          }
          else
          {
               printf( "\
Successfully created the regular file: %s\n\n", name1 );
          }
     }
     else  /* type == 7 */  /* Symbolic link */
     {
          printf( "\
What file would you like to make a symbolic link to?\n" );
          ret = read_stdin( name2, 1026, ">> ", 1 );
          save_errno = errno;
          printf( "\n" );
          if ( ret != 0 )
          {
               printf( "Sorry, something went wrong while reading.\n" );
               if ( save_errno != 0 )
               {
                    printf( "Error: %s.\n", strerror( save_errno ) );
               }
               printf( "\n" );
               return;
          }

          ret = look_for_slash( name2 );
          if ( ret == ( -1 ) )
          {
               printf( "\
Sorry, something went wrong while processing your string.\n\n" );
               return;
          }

          if ( ret != 0 )
          {
               printf( "\
You must not include any forward slash \
characters '/' in your string.\n\n" );
               return;
          }

          /* Find out if the file exists. */

          errno = 0;
          ret = does_file_exist( name2 );
          if ( ret == ( -1 ) )
          {
               save_errno = errno;
               printf( "\
Sorry, something went wrong while looking\n\
for the file: %s\n", name2 );
               if ( save_errno != 0 )
               {
                    printf( "Error: %s.\n", strerror( save_errno ) );
               }
               printf( "\n" );
               return;
          }

          if ( ret == 0 )
          {
               printf( "\
The file \"%s\" was not found in the current directory.\n\n", name2 );
               return;
          }

          /* Make the symbolic link. */

          errno = 0;
          ret = symlink( name2, name1 );
          save_errno = errno;
          if ( ret != 0 )
          {
               printf( "\
Failed to create a symbolic link from \"%s\" to \"%s\"\n",
                       name1, name2 );
               if ( save_errno != 0 )
               {
                    printf( "Error: %s.\n", strerror( save_errno ) );
               }
               printf( "\n" );
               return;
          }

          /* Make sure it worked. */

          errno = 0;
          ret = does_file_exist( name1 );
          if ( ret == ( -1 ) )
          {
               save_errno = errno;
               printf( "\
Sorry, something went wrong while looking\n\
for the symbolic link: %s\n", name1 );
               if ( save_errno != 0 )
               {
                    printf( "Error: %s.\n", strerror( save_errno ) );
               }
               printf( "\n" );
               return;
          }

          if ( ret == 0 )
          {
               printf( "Failed to find the symbolic link: %s\n\n",
                       name1 );
          }
          else
          {
               printf( "\
Successfully created the symbolic link from \"%s\" to \"%s\"\n\n",
                       name1, name2 );
          }
     }
     return;
}

/* This function attempts to read from a file in the current directory. */

void read_from_file( char *cwd )
{
     FILE *fp;
     char ch, last, name[ 1026 ];
     int ret, save_errno;
     struct stat stats;

     if ( cwd == NULL )
     {
          printf( "The current directory is undefined.\n\n" );
          return;
     }
     else
     {
          printf( "Current directory: %s\n\n", cwd );
     }

     printf( "What file would you like to read from?\n" );
     ret = read_stdin( name, 1026, ">> ", 1 );
     save_errno = errno;
     printf( "\n" );
     if ( ret != 0 )
     {
          printf( "Sorry, something went wrong while reading.\n" );
          if ( save_errno != 0 )
          {
               printf( "Error: %s.\n", strerror( save_errno ) );
          }
          printf( "\n" );
          return;
     }

     ret = look_for_slash( name );
     if ( ret == ( -1 ) )
     {
          printf( "\
Sorry, something went wrong while processing your string.\n\n" );
          return;
     }

     if ( ret != 0 )
     {
          printf( "\
You must not include any forward slash \
characters '/' in your string.\n\n" );
          return;
     }

     /* Check to see if the file exists and whether it has any data. */

     init_stat_structure( &stats );
     errno = 0;
     ret = stat( name, &stats );
     if ( ret != 0 )
     {
          save_errno = errno;
          if ( errno == ENOENT )
          {
               printf( "\
The specified file does not exist in the current directory.\n\n" );
          }
          else
          {
               printf( "Failed to stat the file: %s\n", name );
               if ( save_errno != 0 )
               {
                    printf( "Error: %s.\n", strerror( save_errno ) );
               }
               printf( "\n" );
          }
          return;
     }

     if ( stats.st_size == 0 )
     {
          printf( "The specified file is empty.\n\n" );
          return;
     }

     /* Open the file for reading. */

     errno = 0;
     fp = fopen( name, "r" );
     if ( fp == NULL )
     {
          save_errno = errno;
          printf( "Failed to open the file \"%s\" for reading.\n", name );
          if ( save_errno != 0 )
          {
               printf( "Error: %s.\n", strerror( save_errno ) );
          }
          printf( "\n" );
          return;
     }

     /* Read the file a character at a time and print the contents. */

     ch = last = 0;
     while( !feof( fp ) && !ferror( fp ) )
     {
          ch = ( char )fgetc( fp );
          if ( !feof( fp ) && !ferror( fp ) && ch != 0 )
          {
               printf( "%c", ch );
               last = ch;
          }
     }
     fflush( stdout );
     if ( ferror( fp ) )
     {
          clearerr( fp );  /* This clears the end-of-file flag too. */
     }

     /* Close the file. */

     fclose( fp );
     fp = NULL;

     if ( ch != 10 )  /* Was the last printed character a newline? */
     {
          printf( "\n\n" );
     }
     return;
}

/* This function attempts to write to a file in the current directory. */

void write_to_file( char *cwd )
{
     FILE *fp;
     char buffer[ 82 ], name[ 1026 ];
     int choice, exit_loop, ret, save_errno;

     if ( cwd == NULL )
     {
          printf( "The current directory is undefined.\n\n" );
          return;
     }
     else
     {
          printf( "Current directory: %s\n\n", cwd );
     }

     printf( "What file would you like to write to?\n" );
     ret = read_stdin( name, 1026, ">> ", 1 );
     save_errno = errno;
     printf( "\n" );
     if ( ret != 0 )
     {
          printf( "Sorry, something went wrong while reading.\n" );
          if ( save_errno != 0 )
          {
               printf( "Error: %s.\n", strerror( save_errno ) );
          }
          printf( "\n" );
          return;
     }

     ret = look_for_slash( name );
     if ( ret == ( -1 ) )
     {
          printf( "\
Sorry, something went wrong while processing your string.\n\n" );
          return;
     }

     if ( ret != 0 )
     {
          printf( "\
You must not include any forward slash \
characters '/' in your string.\n\n" );
          return;
     }

     /* Open the file in append mode for writing. */

     errno = 0;
     fp = fopen( name, "a" );
     if ( fp == NULL )
     {
          save_errno = errno;
          printf( "Failed to open the file \"%s\" for writing.\n", name );
          if ( save_errno != 0 )
          {
               printf( "Error: %s.\n", strerror( save_errno ) );
          }
          printf( "\n" );
          return;
     }

     /* Write text to the file a line at a time. */

     exit_loop = 0;
     do
     {
          printf( "What would you like to write to the file?\n>> " );
          fflush( stdout );
          errno = 0;

          /* Read the text to be written to the file. */

          if ( fgets( buffer, 82, stdin ) == NULL )
          {
               save_errno = errno;
               printf( "\nSorry, something went wrong while reading.\n" );
               if ( save_errno != 0 )
               {
                    printf( "Error: %s.\n", strerror( save_errno ) );
               }
               printf( "\n" );
               exit_loop = 1;
          }
          else
          {
               save_errno = errno;
               if ( strlen( buffer ) < 1 )
               {
                    printf( "\n\
Sorry, something went wrong while reading.\n" );
                    if ( save_errno != 0 )
                    {
                         printf( "Error: %s.\n", strerror( save_errno ) );
                    }
                    printf( "\n" );
                    exit_loop = 1;
               }
               else
               {
                    /* Write the text to the file. */

                    errno = 0;
                    ret = fprintf( fp, "%s", buffer );
                    save_errno = errno;
                    if ( ret < 1 )
                    {
                         printf( "\n\
Sorry, something went wrong while writing.\n" );
                         if ( save_errno != 0 )
                         {
                              printf( "Error: %s.\n",
                                      strerror( save_errno ) );
                         }
                         printf( "\n" );
                         exit_loop = 1;
                    }
                    else
                    {
                         printf( "\n\
Would you like to write to the file again? (Yes = 1, No = 0)\n" );
                         ret = read_stdin( buffer, 3, ">> ", 1 );
                         save_errno = errno;
                         if ( ret != 0 )
                         {
                              printf( "\n\
Sorry, something went wrong while reading.\n" );
                              if ( save_errno != 0 )
                              {
                                   printf( "Error: %s.\n",
                                           strerror( save_errno ) );
                              }
                              printf( "\n" );
                              exit_loop = 1;
                         }
                         else
                         {
                              if ( sscanf( buffer, "%d", &choice ) != 1 )
                              {
                                   printf( "\n\
Sorry, that is not valid input.\n\n" );
                                   exit_loop = 1;
                              }
                              else
                              {
                                   if ( choice != 1 && choice != 0 )
                                   {
                                        printf( "\n\
Sorry, that is not a valid option.\n\n" );
                                        exit_loop = 1;
                                   }
                                   else if ( choice == 0 )
                                   {
                                        printf( "\n" );
                                        exit_loop = 1;
                                   }
                                   else  /* choice == 1 */
                                   {
                                        printf( "\n" );
                                   } /* if ( choice != 1 && choice != 0 ) */
                              }    /* if ( sscanf( buffer, "%d", ... ) */
                         }    /* if ( ret != 0 ) */
                    }    /* if ( ret < 1 ) */
               }    /* if ( strlen( buffer ) < 1 ) */
          }    /* if ( ret != 0 ) */
     }    while( exit_loop == 0 );

     /* Close the file. */

     if ( ferror( fp ) )
     {
          clearerr( fp );
     }
     fflush( fp );
     if ( ferror( fp ) )
     {
          clearerr( fp );
     }
     fclose( fp );
     fp = NULL;
     return;
}

/* This function attempts to rename a file in the cirrent directory. */

void rename_file( char *cwd )
{
     char new[ 1026 ], old[ 1026 ];
     int ret, save_errno;

     if ( cwd == NULL )
     {
          printf( "The current directory is undefined.\n\n" );
          return;
     }
     else
     {
          printf( "Current directory: %s\n\n", cwd );
     }

     printf( "What file would you like to rename?\n" );
     ret = read_stdin( old, 1026, ">> ", 1 );
     save_errno = errno;
     printf( "\n" );
     if ( ret != 0 )
     {
          printf( "Sorry, something went wrong while reading.\n" );
          if ( save_errno != 0 )
          {
               printf( "Error: %s.\n", strerror( save_errno ) );
          }
          printf( "\n" );
          return;
     }

     ret = look_for_slash( old );
     if ( ret == ( -1 ) )
     {
          printf( "\
Sorry, something went wrong while processing your string.\n\n" );
          return;
     }

     if ( ret != 0 )
     {
          printf( "\
You must not include any forward slash \
characters '/' in your string.\n\n" );
          return;
     }

     errno = 0;
     ret = does_file_exist( old );
     if ( ret == ( -1 ) )
     {
          save_errno = errno;
          printf( "\
Sorry, something went wrong while looking for the file: %s\n", old );
          if ( save_errno != 0 )
          {
               printf( "Error: %s.\n", strerror( save_errno ) );
          }
          printf( "\n" );
          return;
     }

     if ( ret == 0 )
     {
          printf( "\
The specified file does not exist in the current directory.\n\n" );
          return;
     }

     printf( "What would you like to change the file's name to?\n" );
     ret = read_stdin( new, 1026, ">> ", 1 );
     save_errno = errno;
     printf( "\n" );
     if ( ret != 0 )
     {
          printf( "Sorry, something went wrong while reading.\n" );
          if ( save_errno != 0 )
          {
               printf( "Error: %s.\n", strerror( save_errno ) );
          }
          printf( "\n" );
          return;
     }

     ret = look_for_slash( new );
     if ( ret == ( -1 ) )
     {
          printf( "\
Sorry, something went wrong while processing your string.\n\n" );
          return;
     }

     if ( ret != 0 )
     {
          printf( "\
You must not include any forward slash \
characters '/' in your string.\n\n" );
          return;
     }

     errno = 0;
     ret = does_file_exist( new );
     if ( ret == ( -1 ) )
     {
          save_errno = errno;
          printf( "\
Sorry, something went wrong while looking for the file: %s\n", new );
          if ( save_errno != 0 )
          {
               printf( "Error: %s.\n", strerror( save_errno ) );
          }
          printf( "\n" );
          return;
     }

     if ( ret == 1 )
     {
          printf( "\
A file with the new name already exists in the current directory.\n\n" );
          return;
     }

     /* Rename the file or directory. */

     errno = 0;
     ret = rename( old, new );
     if ( ret != 0 )
     {
          save_errno = errno;
          printf( "\
Failed to change the name of the file from \"%s\" to \"%s\"\n",
                  old, new );
          if ( save_errno != 0 )
          {
               printf( "Error: %s.\n", strerror( save_errno ) );
          }
          printf( "\n" );
          return;
     }

     /* Make sure it worked. */

     errno = 0;
     ret = does_file_exist( old );
     if ( ret == ( -1 ) )
     {
          save_errno = errno;
          printf( "\
Sorry, something went wrong while looking for the file: %s\n", old );
          if ( save_errno != 0 )
          {
               printf( "Error: %s.\n", strerror( save_errno ) );
          }
          printf( "\n" );
          return;
     }

     if ( ret == 1 )
     {
          printf( "Failed to change the name of the file: %s\n", old );
          printf( "The file still exists in the current directory.\n\n" );
          return;
     }

     errno = 0;
     ret = does_file_exist( new );
     if ( ret == ( -1 ) )
     {
          save_errno = errno;
          printf( "\
Sorry, something went wrong while looking for the file: %s\n", new );
          if ( save_errno != 0 )
          {
               printf( "Error: %s.\n", strerror( save_errno ) );
          }
          printf( "\n" );
          return;
     }

     if ( ret == 0 )
     {
          /* If we reach this point, the file has been lost. */

          printf( "Failed to change the name of the file: %s\n", old );
          printf( "\
A file with the new name does not exist in the current directory.\n\n" );
          return;
     }

     printf( "The file: %s\nhas been successfully renamed to: %s\n\n",
             old, new );
     return;
}

/* This function attempts to remove a file in the current directory. */

void remove_file( char *cwd )
{
     char buffer[ 1026 ];
     int ret, save_errno;

     if ( cwd == NULL )
     {
          printf( "The current directory is undefined.\n\n" );
          return;
     }
     else
     {
          printf( "Current directory: %s\n\n", cwd );
     }

     printf( "What file would you like to remove?\n" );
     ret = read_stdin( buffer, 1026, ">> ", 1 );
     save_errno = errno;
     printf( "\n" );
     if ( ret != 0 )
     {
          printf( "Sorry, something went wrong while reading.\n" );
          if ( save_errno != 0 )
          {
               printf( "Error: %s.\n", strerror( save_errno ) );
          }
          printf( "\n" );
          return;
     }

     ret = look_for_slash( buffer );
     if ( ret == ( -1 ) )
     {
          printf( "\
Sorry, something went wrong while processing your string.\n\n" );
          return;
     }

     if ( ret != 0 )
     {
          printf( "\
You must not include any forward slash \
characters '/' in your string.\n\n" );
          return;
     }

     errno = 0;
     ret = does_file_exist( buffer );
     if ( ret == ( -1 ) )
     {
          save_errno = errno;
          printf( "\
Sorry, something went wrong while looking for the file: %s\n", buffer );
          if ( save_errno != 0 )
          {
               printf( "Error: %s.\n", strerror( save_errno ) );
          }
          printf( "\n" );
          return;
     }

     if ( ret == 0 )
     {
          printf( "\
The specified file does not exist in the current directory.\n\n" );
          return;
     }

     /* Remove the file. */

     errno = 0;
     ret = unlink( buffer );
     if ( ret != 0 )
     {
          save_errno = errno;
          printf( "Failed to remove the file: %s\n", buffer );
          if ( save_errno != 0 )
          {
               printf( "Error: %s.\n", strerror( save_errno ) );
          }
          printf( "\n" );
          return;
     }

     /* Make sure it worked. */

     errno = 0;
     ret = does_file_exist( buffer );
     if ( ret == ( -1 ) )
     {
          save_errno = errno;
          printf( "\
Sorry, something went wrong while looking for the file: %s\n", buffer );
          if ( save_errno != 0 )
          {
               printf( "Error: %s.\n", strerror( save_errno ) );
          }
          printf( "\n" );
     }
     else if ( ret == 1 )
     {
          printf( "Failed to remove the file: %s\n\n", buffer );
     }
     else  /* ret == 0 */
     {
          printf( "Successfully removed the file: %s\n\n", buffer );
     }
     return;
}

/* This function attempts to create a new directory. */

void create_directory( char *cwd )
{
     char buffer[ 1026 ];
     int ret, save_errno;

     if ( cwd == NULL )
     {
          printf( "The current directory is undefined.\n\n" );
     }
     else
     {
          printf( "Current directory: %s\n\n", cwd );
     }

     printf( "What directory would you like to create?\n" );
     ret = read_stdin( buffer, 1026, ">> ", 1 );
     save_errno = errno;
     printf( "\n" );
     if ( ret != 0 )
     {
          printf( "Sorry, something went wrong while reading.\n" );
          if ( save_errno != 0 )
          {
               printf( "Error: %s.\n", strerror( save_errno ) );
          }
          printf( "\n" );
          return;
     }

     ret = look_for_slash( buffer );
     if ( ret == ( -1 ) )
     {
          printf( "\
Sorry, something went wrong while processing your string.\n\n" );
          return;
     }

     if ( ret != 0 )
     {
          printf( "\
You must not include any forward slash \
characters '/' in your string.\n\n" );
          return;
     }

     /* See if the desired directory name is already in use. */

     errno = 0;
     ret = does_file_exist( buffer );
     if ( ret == ( -1 ) )
     {
          save_errno = errno;
          printf( "\
Sorry, something went wrong while looking for the directory: %s\n",
                  buffer );
          if ( save_errno != 0 )
          {
               printf( "Error: %s.\n", strerror( save_errno ) );
          }
          printf( "\n" );
     }
     else if ( ret == 1 )
     {
          printf( "\
A file or directory with that name already \
exists in the current directory.\n\n" );
          return;
     }

     /* Create the directory. */

     ret = mkdir( buffer, 0777 );
     if ( ret != 0 )
     {
          save_errno = errno;
          printf( "Failed to create the new directory: %s\n", buffer );
          if ( save_errno != 0 )
          {
               printf( "Error: %s.\n", strerror( save_errno ) );
          }
          printf( "\n" );
          return;
     }

     /* Make sure it worked. */

     errno = 0;
     ret = does_file_exist( buffer );
     if ( ret == ( -1 ) )
     {
          save_errno = errno;
          printf( "\
Sorry, something went wrong while looking for the directory: %s\n",
                  buffer );
          if ( save_errno != 0 )
          {
               printf( "Error: %s.\n", strerror( save_errno ) );
          }
          printf( "\n" );
     }
     else if ( ret == 0 )
     {
          printf( "Failed to find the new directory: %s\n\n", buffer );
     }
     else  /* ret == 1 */
     {
          printf( "Successfully created the new directory: %s\n\n",
                  buffer );
     }
     return;
}

/* This function attempts to remove a directory. */

void remove_directory( char *cwd )
{
     char buffer[ 1026 ];
     int ret, save_errno;

     if ( cwd == NULL )
     {
          printf( "The current directory is undefined.\n\n" );
     }
     else
     {
          printf( "Current directory: %s\n\n", cwd );
     }

     printf( "What directory would you like to remove?\n" );
     ret = read_stdin( buffer, 1026, ">> ", 1 );
     save_errno = errno;
     printf( "\n" );
     if ( ret != 0 )
     {
          printf( "Sorry, something went wrong while reading.\n" );
          if ( save_errno != 0 )
          {
               printf( "Error: %s.\n", strerror( save_errno ) );
          }
          printf( "\n" );
          return;
     }

     ret = look_for_slash( buffer );
     if ( ret == ( -1 ) )
     {
          printf( "\
Sorry, something went wrong while processing your string.\n\n" );
          return;
     }

     if ( ret != 0 )
     {
          printf( "\
You must not include any forward slash \
characters '/' in your string.\n\n" );
          return;
     }

     /* See if the directory exists. */

     errno = 0;
     ret = does_file_exist( buffer );
     if ( ret == ( -1 ) )
     {
          save_errno = errno;
          printf( "\
Sorry, something went wrong while looking for the directory: %s\n",
                  buffer );
          if ( save_errno != 0 )
          {
               printf( "Error: %s.\n", strerror( save_errno ) );
          }
          printf( "\n" );
     }
     else if ( ret == 0 )
     {
          printf( "\
The directory: %s\ndoes not exist in the current directory.\n\n", buffer );
          return;
     }

     /* Remove the directory. */

     ret = rmdir( buffer );
     if ( ret != 0 )
     {
          save_errno = errno;
          printf( "Failed to remove the directory: %s\n", buffer );
          if ( save_errno != 0 )
          {
               printf( "Error: %s.\n", strerror( save_errno ) );
          }
          printf( "\n" );
          return;
     }

     /* Make sure it worked. */

     errno = 0;
     ret = does_file_exist( buffer );
     if ( ret == ( -1 ) )
     {
          save_errno = errno;
          printf( "\
Sorry, something went wrong while looking for the directory: %s\n",
                  buffer );
          if ( save_errno != 0 )
          {
               printf( "Error: %s.\n", strerror( save_errno ) );
          }
          printf( "\n" );
     }
     else if ( ret == 1 )
     {
          printf( "Failed to remove the directory: %s\n\n", buffer );
     }
     else  /* ret == 0 */
     {
          printf( "Successfully removed the directory: %s\n\n", buffer );
     }
     return;
}

/*

     This function looks for a forward slash '/' character in a string.

     Returns 1 if it finds at least one forward slash character,
     0 if it doesn't find any, or -1 if an error occurs.

*/

int look_for_slash( char *str )
{
     int pos, size;

     if ( str == NULL || str[ 0 ] == 0 )
     {
          return ( -1 );
     }
     size = strlen( str );
     for( pos = 0; pos < size; pos++ )
     {
          if ( str[ pos ] == '/' )
          {
               return 1;
          }
     }
     return 0;
}

/* EOF files.c */
