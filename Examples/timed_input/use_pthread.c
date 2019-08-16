/*

     use_pthread.c

     This program uses a child thread to
     offer a time limit for user input.

     This program must use the -pthread compiler option.

     Written by Matthew Campbell on Tuesday July 23, 2019.

*/

/* sigaction(2) needs this. */

#define _POSIX_C_SOURCE 1

/* Define WAIT_FOR_CHILD if you want to call waitpid(2). */

#undef WAIT_FOR_CHILD

#ifdef WAIT_FOR_CHILD

/* waitpid(2) needs this. */

#define _XOPEN_SOURCE 500

#endif

/* Include what we need: */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#ifdef WAIT_FOR_CHILD

#include <sys/wait.h>
#include <sys/types.h>

#endif

/* Make sure this is defined: */

#ifndef SIGALRM
#define SIGARLM 14
#endif

/* This is the length of the buffer that holds the user's input. */

#define BUFFER_SIZE 80

/*

     Define SHOW_WAIT_ERRORS if you want
     to display errors from waitpid(2).

*/

#ifdef WAIT_FOR_CHILD

#define SHOW_WAIT_ERRORS

#else

#undef SHOW_WAIT_ERRORS

#endif

/* Function prototypes: */

/* This function receives the SIGALRM signal from the child thread. */

void catch_alarm_signal( int signum );

/* The child thread will call this to gather input from the user. */

void *child_thread( void *arg );

/* Variables and definitions: */

/* This structure will be used to pass data between threads. */

struct arg_t
{
     char  buffer[ BUFFER_SIZE ];
     pid_t ppid;
     int   ret;
     int   save_errno;
};

/* The child thread will use this for its stack space. */

unsigned char child_stack[ ( 1024 * 1024 ) ];  /* 1 megabyte */

/* Function definitions: */

int main( void )
{
     int answer, ret, save_errno;
     pthread_t thread;
     struct arg_t args;
     struct sigaction alarm_handler;

     /* Set up the signal handling function. */

     /* Clear the data space. */

     memset( &alarm_handler, 0, sizeof( struct sigaction ) );

     /* Tell it what function to call when we receive the signal. */

     alarm_handler.sa_handler = catch_alarm_signal;

     /* Activate the new signal handler. */

     errno = 0;
     ret = sigaction( SIGALRM, &alarm_handler, NULL );
     save_errno = errno;
     if ( ret != 0 )
     {
          printf( "\
Something went wrong when trying to setup catch_alarm_signal().\n" );
          if ( save_errno != 0 )
          {
               printf( "Error: %s.\n", strerror( save_errno ) );
          }
          printf( "\n" );
          exit( EXIT_FAILURE );
     }

     /* Clear the structure's data space. */

     memset( &args, 0, sizeof( struct arg_t ) );

     /* Save the pid of the main process. */

     args.ppid = getpid();

     /* Print the question and available answers. */

     printf( "\
You have five seconds to correctly answer this question.\n\n" );

     printf( "What sound does the duck make?\n\n" );
     printf( "1) Meow\n" );
     printf( "2) Woof\n" );
     printf( "3) Quack\n" );
     printf( "4) Moo\n\n" );
     printf( ">> " );
     fflush( stdout );

     /* Create the child thread. */

     ret = pthread_create( &thread, NULL, &child_thread,
                           ( void * )( &args ) );

     /* pthread_create(3) returns the error code this way. */

     if ( ret != 0 )
     {
          printf( "\n\n\
Something went wrong when trying to create the child thread.\n" );
          printf( "Error: %s.\n\n", strerror( ret ) );
          exit( EXIT_FAILURE );
     }

     /* Wait five seconds so the child thread can gather the input. */

     sleep( 5 );

     /* Did anything go wrong while reading the input? */

     if ( args.ret != 0 )
     {
          printf( "Something went wrong while reading your input.\n" );
          if ( args.save_errno != 0 )
          {
               printf( "Error: %s.\n", strerror( args.save_errno ) );
          }
          printf( "\n" );

#ifdef WAIT_FOR_CHILD

          errno = 0;
          ret = waitpid( ( -1 ), NULL, 0 );
          save_errno = errno;

#ifdef SHOW_WAIT_ERRORS

          if ( ret == ( -1 ) )
          {
               printf( "\
Something went wrong when waiting for the child thread.\n" );
               if ( save_errno != 0 )
               {
                    printf( "Error: %s.\n", strerror( save_errno ) );
               }
               printf( "\n" );
          }

#endif

#endif

          printf( "Exiting.\n\n" );
          exit( EXIT_FAILURE );
     }

     /* Did we receive a string from the user? */

     if ( args.buffer[ 0 ] == 0 )
     {
          /*

               Move to the beginning of the line
               and clear to the whole line.

          */

          printf( "%c%c[2K", 13, 27 );

          printf( "\
Time's up.  You did not provide an answer within \
the available time.\n\n" );

#ifdef WAIT_FOR_CHILD

          errno = 0;
          ret = waitpid( ( -1 ), NULL, 0 );
          save_errno = errno;

#ifdef SHOW_WAIT_ERRORS

          if ( ret == ( -1 ) )
          {
               printf( "\
Something went wrong when waiting for the child thread.\n" );
               if ( save_errno != 0 )
               {
                    printf( "Error: %s.\n", strerror( save_errno ) );
               }
               printf( "\n" );
          }

#endif

#endif

          printf( "Exiting.\n\n" );
          exit( EXIT_FAILURE );
     }

     printf( "\n" );

     /* Did the user only press Enter? */

     if ( args.buffer[ 0 ] == '\n' )
     {
          printf( "You didn't choose an answer.\n\n" );

#ifdef WAIT_FOR_CHILD

          errno = 0;
          ret = waitpid( ( -1 ), NULL, 0 );
          save_errno = errno;

#ifdef SHOW_WAIT_ERRORS

          if ( ret == ( -1 ) )
          {
               printf( "\
Something went wrong when waiting for the child thread.\n" );
               if ( save_errno != 0 )
               {
                    printf( "Error: %s.\n", strerror( save_errno ) );
               }
               printf( "\n" );
          }

#endif

#endif

          printf( "Exiting.\n\n" );
          exit( EXIT_FAILURE );
     }

     /* Find out if the user provided a useful response. */

     if ( sscanf( args.buffer, "%d", &answer ) != 1 )
     {
          printf( "That is not valid input.\n\n" );

#ifdef WAIT_FOR_CHILD

          errno = 0;
          ret = waitpid( ( -1 ), NULL, 0 );
          save_errno = errno;

#ifdef SHOW_WAIT_ERRORS

          if ( ret == ( -1 ) )
          {
               printf( "\
Something went wrong when waiting for the child thread.\n" );
               if ( save_errno != 0 )
               {
                    printf( "Error: %s.\n", strerror( save_errno ) );
               }
               printf( "\n" );
          }

#endif

#endif

          printf( "Exiting.\n\n" );
          exit( EXIT_FAILURE );
     }

     if ( answer < 1 || answer > 4 )
     {
          printf( "That is not an available choice.\n\n" );

#ifdef WAIT_FOR_CHILD

          errno = 0;
          ret = waitpid( ( -1 ), NULL, 0 );
          save_errno = errno;

#ifdef SHOW_WAIT_ERRORS

          if ( ret == ( -1 ) )
          {
               printf( "\
Something went wrong when waiting for the child thread.\n" );
               if ( save_errno != 0 )
               {
                    printf( "Error: %s.\n", strerror( save_errno ) );
               }
               printf( "\n" );
          }

#endif

#endif

          printf( "Exiting.\n\n" );
          exit( EXIT_FAILURE );
     }

     /* Did the user provide the correct answer? */

     if ( answer == 3 )
     {
          printf( "Your answer is correct.\n\n" );

#ifdef WAIT_FOR_CHILD

          errno = 0;
          ret = waitpid( ( -1 ), NULL, 0 );
          save_errno = errno;

#ifdef SHOW_WAIT_ERRORS

          if ( ret == ( -1 ) )
          {
               printf( "\
Something went wrong when waiting for the child thread.\n" );
               if ( save_errno != 0 )
               {
                    printf( "Error: %s.\n", strerror( save_errno ) );
               }
               printf( "\n" );
          }

#endif

#endif

          printf( "Successful exit.\n\n" );
          exit( EXIT_SUCCESS );
     }
     else
     {
          printf( "You chose the wrong answer.\n\n" );

#ifdef WAIT_FOR_CHILD

          errno = 0;
          ret = waitpid( ( -1 ), NULL, 0 );
          save_errno = errno;

#ifdef SHOW_WAIT_ERRORS

          if ( ret == ( -1 ) )
          {
               printf( "\
Something went wrong when waiting for the child thread.\n" );
               if ( save_errno != 0 )
               {
                    printf( "Error: %s.\n", strerror( save_errno ) );
               }
               printf( "\n" );
          }

#endif

#endif

          printf( "Exiting.\n\n" );
          exit( EXIT_FAILURE );
     }

     /* And we're done. */
}

void *child_thread( void *arg )
{
     struct arg_t *args;

     /* Did we receive the structure? */

     if ( arg == NULL )
     {
          pthread_exit( EXIT_FAILURE );
     }

     args = ( struct arg_t * )arg;

     errno = 0;
     if ( fgets( args->buffer, BUFFER_SIZE, stdin ) == NULL )
     {
          args->save_errno = errno;
          args->ret = 1;
     }

     /* Wake up the main process. */

     /*

          kill(2) sends a signal, which is often used to end
          a process, but it can send other signals too.  In
          this case it sends alarm which will wake up the
          main process that is currently sleeping.

     */

     kill( args->ppid, SIGALRM );

     /* And exit. */

     pthread_exit( EXIT_SUCCESS );
}

/* It really doesn't do much, but we have to have something. */

void catch_alarm_signal( int signum )
{
     return;
}

/* EOF use_pthread.c */
