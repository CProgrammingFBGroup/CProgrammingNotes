/*

     timed_input.c

     This program uses a child thread to
     offer a time limit for user input.

     Written by Matthew Campbell on Sunday July 7, 2019.

*/

/* clone(2) needs this. */

#define _GNU_SOURCE

/* sigaction(2) needs this. */

#define _POSIX_C_SOURCE 1

/* Include what we need. */

#include <errno.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

/* Make sure these are defined. */

#ifndef SIGALRM
#define SIGARLM 14
#endif

#ifndef SIGTERM
#define SIGTERM 15
#endif

/* This is the length of the buffer that holds the user's input. */

#define BUFFER_SIZE 80

/* Function prototypes: */

/* This function receives the SIGALRM signal from the child thread. */

void catch_alarm_signal( int signum );

/* The child thread will call this to gather input from the user. */

int child_thread( void *arg );

/* Variables and definitions: */

/* This structure will be used to pass data between threads. */

struct arg_t
{
     char buffer[ BUFFER_SIZE ];
     int ret;
     int save_errno;
};

/* The child thread will use this for its stack space. */

unsigned char child_stack[ ( 1024 * 1024 ) ];  /* 1 megabyte */

/* Function definitions: */

int main( void )
{
     unsigned char *stack_pointer;
     int answer, ret, save_errno;
     pid_t pid;
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

     /* Set up the stack space for the child thread. */

     stack_pointer = ( unsigned char * )( ( unsigned int )( child_stack ) +
                                          ( 1024 * 1024 ) );

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

     /*

          We do not use CLONE_FILES so any stray input
          in stdin goes away when the child thread exits.

     */

     errno = 0;           /* CLONE_VM uses shared virtual memory. */
     pid = clone( child_thread, stack_pointer, CLONE_VM,
                  ( void * )( &args ) );
     save_errno = errno;

     if ( pid == ( -1 ) )
     {
          printf( "\
Something wrong when trying to create the child thread.\n" );
          if ( save_errno != 0 )
          {
               printf( "Error: %s.\n", strerror( save_errno ) );
          }
          printf( "\n" );
          exit( EXIT_FAILURE );
     }

     /* Wait five seconds so the child thread can gather the input. */

     sleep( 5 );

     /* Shut down the child thread. */

     kill( pid, SIGTERM );

     /* Did anything go wrong while reading the input? */

     if ( args.ret != 0 )
     {
          printf( "Something went wrong while reading your input.\n" );
          if ( args.save_errno != 0 )
          {
               printf( "Error: %s.\n", strerror( args.save_errno ) );
          }
          printf( "\n" );
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
          exit( EXIT_FAILURE );
     }

     printf( "\n" );

     /* Did the user only press Enter? */

     if ( args.buffer[ 0 ] == '\n' )
     {
          printf( "You didn't choose an answer.\n\n" );
          exit( EXIT_FAILURE );
     }

     /* Find out if the user provided a useful redponse. */

     if ( sscanf( args.buffer, "%d", &answer ) != 1 )
     {
          printf( "That is not valid input.\n\n" );
          exit( EXIT_FAILURE );
     }

     if ( answer < 1 || answer > 4 )
     {
          printf( "That is not an available choice.\n\n" );
          exit( EXIT_FAILURE );
     }

     /* Did the user provide the correct answer? */

     if ( answer == 3 )
     {
          printf( "Your answer is correct.\n\n" );
          exit( EXIT_SUCCESS );
     }
     else
     {
          printf( "You chose the wrong answer.\n\n" );
          exit( EXIT_FAILURE );
     }

     /* And we're done. */
}

int child_thread( void *arg )
{
     int save_errno;
     pid_t pid;
     struct arg_t *args;

     /* Did we receive the structure? */

     if ( arg == NULL )
     {
          /* We have no way of telling the main process. */

          return 1;
     }

     args = ( struct arg_t * )arg;

     errno = 0;
     if ( fgets( args->buffer, BUFFER_SIZE, stdin ) == NULL )
     {
          save_errno = errno;
          args->ret = 1;
          args->save_errno = save_errno;
     }

     /* Wake up the main process. */

     pid = getppid();
     if ( pid >= 0 )
     {
          /*

               kill(2) sends a signal, which is often used to end
               a process, but it can send other signals too.  In
               this case it sends alarm which will wake up the
               main process that is currently sleeping.

          */

          kill( pid, SIGALRM );
     }

     /* Do not call exit(2). */

     return 0;
}

/* It really does do much, but we had to have something. */

void catch_alarm_signal( int signum )
{
     return;
}

/* EOF timed_input.c */
