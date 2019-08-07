/*

     factorial.c

     Finds the factorial of a number as high as
     20 with a recursive or an iterative solution.

     This program has been provided as an example to help
     beginner and intermediate level programmers in C to
     understand how recursion works.  Every call to printf()
     uses a different number in parentheses to help the user
     find that specific location in the source code.

     This is an open source program that may be freely copied
     and distributed.  It has been provided for educational
     purposes.

     Written by Matthew Campbell on Wednesday August 15, 2018.

*/

#include <stdio.h>

/*

     Define USE_RECURSION to use a recursive solution or
     define USE_ITERATION to use an iterative solution.

*/

#define USE_RECURSION
#undef USE_ITERATION

/* Make sure both options aren't selected. */

#ifdef USE_RECURSION
#ifdef USE_ITERATION
#error Only define USE_RECURSION or USE_ITERATION, not both.
#endif
#endif

/* Set a default value if nothing was selected. */

#ifndef USE_RECURSION
#ifndef USE_ITERATION
#define USE_RECURSION
#endif
#endif

/* Now we can use ulli_t instead of writing unsigned long long int. */

typedef unsigned long long int ulli_t;

/* Prototypes: */

#ifdef USE_RECURSION

ulli_t factorial( ulli_t number, ulli_t level );

#endif

#ifdef USE_ITERATION

ulli_t factorial( ulli_t number );

#endif

/* Function definitions: */

int main( void )
{
     int num;
     ulli_t answer, number;

     printf( "\
(1) main(): What number would you like to find the factorial of?\n\
>> " );

     /* We need to call fflush() because the string didn't end with \n */

     fflush( stdout );

     if ( scanf( "%d", &num ) != 1 )  /* Input was not an integer. */
     {
          printf( "\
(2) main(): That is not an integer number.\n\n" );

          return 1;  /* Error. */
     }

     if ( num < 1 )
     {
          printf( "\n\
(3) main(): The number must be greater than zero.\n\n" );

          return 1;  /* Error. */
     }

     if ( num > 20 )
     {
          printf( "\n\
(4) main(): Using a number greater than 20 will overflow the variable.\
\n\n" );

          return 1;  /* Error. */
     }

     /* Now that we know we have safe input we can find the solution. */

     number = ( ulli_t )num;

#ifdef USE_RECURSION

     printf( "\n\
(5) main(): Calling factorial( number: %llu, level: 1 ).\n\n", number );

     answer = factorial( number, 1 );  /* Here we go... */

     printf( "\n\
(6) main(): Received answer: %llu.\n\n", answer );

#endif  /* USE_RECURSION */

#ifdef USE_ITERATION

     printf( "\n\
(7) main(): Calling factorial( number: %llu ).\n\n", number );

     answer = factorial( number );  /* Here we go... */

     printf( "\
(8) main(): Received answer: %llu.\n\n", answer );

#endif  /* USE_ITERATION */

     printf( "(9) main(): Exiting.\n\n" );

     return 0;  /* Successful program execution and exit. */
}

#ifdef USE_RECURSION

/*

     This is the function that recursively
     finds the factorial of a number.

     The variable 'level' is used to keep track of how many times this
     function has been called.  It is printed inside the parentheses
     as factorial(level) for each call to printf().

     Each time a recursive function is called the program will build
     a new stack frame for the function.  When the stop condition is
     reached, those stack frames will be removed, one by one.

*/

ulli_t factorial( ulli_t number, ulli_t level )
{
     ulli_t answer;

     printf( "\
(10) factorial(%llu): Entered the recursive version of factorial().\
\n", level );

     if ( number < 1 )  /* if ( number == 0 ) */
     {
          printf( "\n\
(11) factorial(%llu): Invalid argument received: number: %llu.\n\n",
                  level, number );

          return 0;  /* Error. */
     }

     if ( level < 1 )  /* if ( level == 0 ) */
     {
          printf( "\n\
(12) factorial(%llu): Invalid argument received: level: %llu.\n\n",
                  level, number );

          return 0;  /* Error. */
     }

     /*

          First we need a stop condition.  Without a stop condition
          a recursive function will continue to call itself over and
          over again until the computer runs out of resources which
          will cause the program to crash.

     */

     if ( number == 1 )
     {
          printf( "\
(13) factorial(%llu): Stop condition reached.  Returning answer: 1.\n",
                  level );

          /*

               'number' is 1 so we return 1.  A program doesn't
               necessarily return 1 when it reaches the stop
               condition.  This program does because of the way
               this program's algorithm works.

          */

          return 1;
     }

     /*

          The second thing we need is another call to this function
          with different variables so the function does something
          useful.  If it does nothing useful, then it is worthless.

     */

     else
     {
          printf( "\
(14) factorial(%llu): Calling this function \
again using number: %llu and level: %llu\n",
                  level, ( number - 1 ), ( level + 1 ) );

          /* answer = number * ( number - 1 ) * ( number - 2 ) ... */

          answer = number * factorial( ( number - 1 ), ( level + 1 ) );

          printf( "\
(15) factorial(%llu): Returning answer: %llu.\n", level, answer );

          return answer;
     }
}

#endif  /* USE_RECURSION */

#ifdef USE_ITERATION

/* This is the function that iteratively finds the factorial of a number. */

ulli_t factorial( ulli_t number )
{
     ulli_t answer, count;

     printf( "\
(16) factorial() Entered the iterative version of factorial().\n\n" );

     if ( number < 1 )  /* if ( number == 0 ) */
     {
          printf( "(17) factorial(): Invalid argument received: %llu.\n\n",
                  number );

          return 0;  /* Error. */
     }

     answer = 1;
     for( count = number; count > 0; count-- )
     {
          printf( "\
(18) factorial(): answer: %llu = answer * count: %llu\n", answer, count );

          answer = answer * count;
     }

     printf( "\n\
(19) factorial() returning answer: %llu from factorial().\n\n", answer );

     return answer;
}

#endif  /* USE_ITERATION */

/* EOF factorial.c */
