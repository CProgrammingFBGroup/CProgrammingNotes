/*

     convert.c

     Converts between metric and English measurements.

     This program was written to help provide
     an example of good programming style.

     This program may be freely copied and distributed.

     Written by Matthew Campbell (1-31-18)

*/

/* Include what we need... */

#include <math.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* List function prototypes... */

/* The functions are listed here in the same */
/* order they are defined in in the file.    */

/* Notice the indentation for the arguments passed to this function. */

/* We want everything to line up nice and neat. */

int get_input( const char *prompt, const double min_value,
               const double max_value, const int use_int, int *choice,
               double *value );

void list_menu( void );
void show_answer( const double answer, const char *units );

double inches_to_centimeters( const double inches );
double centimeters_to_inches( const double centimeters );
double feet_to_meters( const double feet );
double meters_to_feet( const double meters );
double miles_to_kilometers( const double miles );
double kilometers_to_miles( const double kilometers );
double fahrenheit_to_celsius( const double fahrenheit );
double celsius_to_fahrenheit( const double celsius );
double pounds_to_kilograms( const double pounds );
double kilograms_to_pounds( const double kilograms );
double ounces_to_grams( const double ounces );
double grams_to_ounces( const double grams );
double fluid_ounces_to_milliliters( const double fluid_ounces );
double milliliters_to_fluid_ounces( const double milliliters );
double usgallons_to_liters( const double usgallons );
double liters_to_usgallons( const double liters );

/*

     All of the local variables used in a function should be declared after
     the opening curly brace instead of being distributed throughout the
     function.  Spreading them out in the function can make it more difficult
     to follow and understand it.  Just because something works doesn't
     necessarily mean it should be used.  Declaring the variables all
     together at the beginning allows the compiler to allocate space for
     them with a single operation to adjust the stack pointer register once
     instead of having to go back and adjust again and again to accommodate
     new variables as they are encountered.  Declaring a variable inside any
     pair of curly braces limits the scope of that variable to that space.
     So if you declare a variable inside an if statement:

     if ( condition )
     {
          int num;

          num = 3;

          (Other code goes here...)
     }

     At this point, now that the matching closing curly brace has been used,
     num is no longer declared and cannot be accessed.  Some people just
     declare a variable wherever they decide they need it.  This, in my
     opinion, produces sloppy looking code.  Perhaps such a person is too
     lazy to put the local variable declaration near the top of the function
     where it belongs.

     Code should be indented maybe four or five spaces per grouping.  Some
     people use a tab character instead of spaces, but I do not recommend
     this because someone else's tab stops may be different than yours.
     The user may be able to set them wherever they want them.  This
     difference can produce unpredictable results when viewing the code.
     Just pick a number of spaces that makes your code look neat and
     orderly and then be consistent.  Don't be afraid to use a little open
     space every here and there so your code won't look congested or
     crowded.  Always use plenty of comments to explain what you are doing.

     Pay attention to how my code is indented.  When I use an if statement
     or a loop a new code grouping is created so that grouping is indented
     again:

     do
     {
          num = 5;
          if ( condition_exists )
          {
               function1( &num );
               printf( "Returned from function1().\n" );
          }
     }    while( exit_loop == 0 );

     Obviously this does not include a whole lot since it is just a simple
     example.  Try to use a monospace font in your text editor that you use
     when reading or writing source code so each character uses the same
     amount of space so everything can be lined up to look nice and neat.

     Notice the spaces around the parentheses.  Everything looks open
     rather than crowded or compressed.  It's ok to use some extra space.
     File systems have plenty of room these days and it really won't make
     your file take up that much more room.  It can help make your code look
     more open and relaxed.

     When you have one closing curly brace on each line, one after another,
     use some comments to tell the person reading the code what each closing
     curly brace matches up with:

               }    // for()
          }    // if ( condition_exists )
     }    // while()

     I had to use inline comments here because my compiler would not allow
     me to use nested comments.  You should use the tradtional comment style.

     Some people will place the opening curly brace after whatever uses it:

     if ( condition ) {
          function1( num );
     }

     I prefer to put it on the next line because I believe it adds symmetry
     to the code and can help make it look less crowded:

     if ( condition )
     {
          function1( num );
     }

     Technically you do not need to use the curly braces if only one
     statement would be used inside them.  I use them anyway because I
     like the structured look I believe they provide.  So:

     if ( condition ) function1( num );

     does the same thing as:

     if ( condition )
     {
          function1( num );
     }

     Each programmer will have their own personal preferences and style.
     It's up to you to make sure you write high quality code that others
     can eaily read and understand.  What you write says something about
     you.  Are you the type of person that does quality work that others
     can respect, or do you just quickly throw something together without
     caring about how sloppy it looks as long as it runs?  Something
     worth doing is worth doing right.  You might as well do a good job
     that you can be proud of when you are finished.

*/

/* Notice that my variable declarations after the */
/* opening curly brace are in alphabetical order. */

int main( void )
{
     char buffer[ 3 ];
     double answer, value;
     int choice, exit_loop, ret, save_errno;

     /* Always initialize your variables before you try to use them. */

     exit_loop = ret = 0;

     /* The others will be initialized later. */

     do
     {
          do
          {

               /* List the menu for the user. */

               /* Suppress the menu if we only read a '\n'. */

               if ( ret != 1 )
               {
                    list_menu();
               }

               /* Read the user's choice.  Here ret  */
               /* holds the function's return value. */

               ret = get_input( ">> ", 1.0, 17.0, 1, &choice, &value );

               /* The function get_input() will return 1 or -1 if   */
               /* something went wrong so looping around again will */
               /* give the user the chance to correct the problem.  */

          }    while( ret != 0 );

          /* Now we know what the user wants to */
          /* do and that the choice is valid.   */

          printf( "\n" );
          switch( choice )
          {
               case  1: do
                        {
                             ret = get_input( "Number of inches? >> ",
                                              0.0, 12000.0, 0, &choice,
                                              &value );
                        }    while( ret != 0 );
                        answer = inches_to_centimeters( value );

                        /* You might be surprised by just how picky   */
                        /* floating point numbers can be.  So we have */
                        /* to test each of them this way.             */

                        if ( answer >= 0.999999999999999 &&
                             answer <= 1.00000000000001 )
                        {
                             answer = 1.0;
                             show_answer( answer, "centimeter" );
                        }
                        else
                        {
                             show_answer( answer, "centimeters" );
                        }
                        break;

               case  2: do
                        {
                             ret = get_input( "Number of centimeters? >> ",
                                              0.0, 30480.0, 0, &choice,
                                              &value );
                        }    while( ret != 0 );
                        answer = centimeters_to_inches( value );
                        if ( answer >= 0.999999999999999 &&
                             answer <= 1.00000000000001 )
                        {
                             answer = 1.0;
                             show_answer( answer, "inch" );
                        }
                        else
                        {
                             show_answer( answer, "inches" );
                        }
                        break;

               case  3: do
                        {
                             ret = get_input( "Number of feet? >> ",
                                              0.0, 528000.0, 0, &choice,
                                              &value );
                        }    while( ret != 0 );
                        answer = feet_to_meters( value );
                        if ( answer >= 0.999999999999999 &&
                             answer <= 1.00000000000001 )
                        {
                             answer = 1.0;
                             show_answer( answer, "meter" );
                        }
                        else
                        {
                             show_answer( answer, "meters" );
                        }
                        break;

               case  4: do
                        {
                             ret = get_input( "Number of meters? >> ",
                                              0.0, 160935.0, 0, &choice,
                                              &value );
                        }    while( ret != 0 );
                        answer = meters_to_feet( value );
                        if ( answer >= 0.999999999999999 &&
                             answer <= 1.00000000000001 )
                        {
                             answer = 1.0;
                             show_answer( answer, "foot" );
                        }
                        else
                        {
                             show_answer( answer, "feet" );
                        }
                        break;

               case  5: do
                        {
                             ret = get_input( "Number of miles? >> ",
                                              0.0, 1000000.0, 0, &choice,
                                              &value );
                        }    while( ret != 0 );
                        answer = miles_to_kilometers( value );
                        if ( answer >= 0.999999999999999 &&
                             answer <= 1.00000000000001 )
                        {
                             answer = 1.0;
                             show_answer( answer, "kilometer" );
                        }
                        else
                        {
                             show_answer( answer, "kilometers" );
                        }
                        break;

               case  6: do
                        {
                             ret = get_input( "Number of kilometers? >> ",
                                              0.0, 1609344.0, 0, &choice,
                                              &value );
                        }    while( ret != 0 );
                        answer = kilometers_to_miles( value );
                        if ( answer >= 0.999999999999999 &&
                             answer <= 1.00000000000001 )
                        {
                             answer = 1.0;
                             show_answer( answer, "mile" );
                        }
                        else
                        {
                             show_answer( answer, "miles" );
                        }
                        break;

               case  7: do
                        {
                             ret = get_input( "\
Number of degrees Fahrenheit? >> ",
                                              -459.67, 1800032.0, 0,
                                              &choice, &value );
                        }    while( ret != 0 );
                        answer = fahrenheit_to_celsius( value );
                        if ( answer >= 0.999999999999999 &&
                             answer <= 1.00000000000001 )
                        {
                             answer = 1.0;
                             show_answer( answer, "degree Celsius" );
                        }
                        else if ( answer <= ( -0.999999999999999 ) &&
                                  answer >= ( -1.00000000000001 ) )
                        {
                             answer = ( -1.0 );
                             show_answer( answer, "degree Celsius" );
                        }
                        else
                        {
                             show_answer( answer, "degrees Celsius" );
                        }
                        break;

               case  8: do
                        {
                             ret = get_input( "\
Number of degrees Celsius? >> ",
                                              -273.15, 1000000.0, 0,
                                              &choice, &value );
                        }    while( ret != 0 );
                        answer = celsius_to_fahrenheit( value );
                        if ( answer >= 0.999999999999999 &&
                             answer <= 1.00000000000001 )
                        {
                             answer = 1.0;
                             show_answer( answer, "degree Fahrenheit" );
                        }
                        else if ( answer <= ( -0.999999999999999 ) &&
                                  answer >= ( -1.00000000000001 ) )
                        {
                             answer = ( -1.0 );
                             show_answer( answer, "degree Fahrenheit" );
                        }
                        else
                        {
                             show_answer( answer, "degrees Fahrenheit" );
                        }
                        break;

               case  9: do
                        {
                             ret = get_input( "Number of pounds? >> ",
                                              0.0, 2204623.0, 0, &choice,
                                              &value );
                        }    while( ret != 0 );
                        answer = pounds_to_kilograms( value );
                        if ( answer >= 0.999999999999999 &&
                             answer <= 1.00000000000001 )
                        {
                             answer = 1.0;
                             show_answer( answer, "kilogram" );
                        }
                        else
                        {
                             show_answer( answer, "kilograms" );
                        }
                        break;

               case 10: do
                        {
                             ret = get_input( "Number of kilograms? >> ",
                                              0.0, 1000000.0, 0, &choice,
                                              &value );
                        }    while( ret != 0 );
                        answer = kilograms_to_pounds( value );
                        if ( answer >= 0.999999999999999 &&
                             answer <= 1.00000000000001 )
                        {
                             answer = 1.0;
                             show_answer( answer, "pound" );
                        }
                        else
                        {
                             show_answer( answer, "pounds" );
                        }
                        break;

               case 11: do
                        {
                             ret = get_input( "Number of ounces? >> ",
                                              0.0, 35274.0, 0, &choice,
                                              &value );
                        }    while( ret != 0 );
                        answer = ounces_to_grams( value );
                        if ( answer >= 0.999999999999999 &&
                             answer <= 1.00000000000001 )
                        {
                             answer = 1.0;
                             show_answer( answer, "gram" );
                        }
                        else
                        {
                             show_answer( answer, "grams" );
                        }
                        break;

               case 12: do
                        {
                             ret = get_input( "Number of grams? >> ",
                                              0.0, 1000000.0, 0, &choice,
                                              &value );
                        }    while( ret != 0 );
                        answer = grams_to_ounces( value );
                        if ( answer >= 0.999999999999999 &&
                             answer <= 1.00000000000001 )
                        {
                             answer = 1.0;
                             show_answer( answer, "ounce" );
                        }
                        else
                        {
                             show_answer( answer, "ounces" );
                        }
                        break;

               case 13: do
                        {
                             ret = get_input( "Number of fluid ounces? >> ",
                                              0.0, 33815.0, 0, &choice,
                                              &value );
                        }    while( ret != 0 );
                        answer = fluid_ounces_to_milliliters( value );
                        if ( answer >= 0.999999999999999 &&
                             answer <= 1.00000000000001 )
                        {
                             answer = 1.0;
                             show_answer( answer, "milliliter" );
                        }
                        else
                        {
                             show_answer( answer, "milliliters" );
                        }
                        break;

               case 14: do
                        {
                             ret = get_input( "Number of milliliters? >> ",
                                              0.0, 1000000.0, 0, &choice,
                                              &value );
                        }    while( ret != 0 );
                        answer = milliliters_to_fluid_ounces( value );
                        if ( answer >= 0.999999999999999 &&
                             answer <= 1.00000000000001 )
                        {
                             answer = 1.0;
                             show_answer( answer, "fluid ounce" );
                        }
                        else
                        {
                             show_answer( answer, "fluid ounces" );
                        }
                        break;

               case 15: do
                        {
                             ret = get_input( "Number of US gallons? >> ",
                                              0.0, 1000.0, 0, &choice,
                                              &value );
                        }    while( ret != 0 );
                        answer = usgallons_to_liters( value );
                        if ( answer >= 0.999999999999999 &&
                             answer <= 1.00000000000001 )
                        {
                             answer = 1.0;
                             show_answer( answer, "liter" );
                        }
                        else
                        {
                             show_answer( answer, "liters" );
                        }
                        break;

               case 16: do
                        {
                             ret = get_input( "Number of liters? >> ",
                                              0.0, 3786.0, 0, &choice,
                                              &value );
                        }    while( ret != 0 );
                        answer = liters_to_usgallons( value );
                        if ( answer >= 0.999999999999999 &&
                             answer <= 1.00000000000001 )
                        {
                             answer = 1.0;
                             show_answer( answer, "gallon" );
                        }
                        else
                        {
                             show_answer( answer, "gallons" );
                        }
                        break;

              case 17 : exit_loop = 1;
                        break;

                        /* Reaching the default case condition in       */
                        /* this program should be logically impossible. */

              default : printf( "Something went wrong.\n" );
                        printf( "\
The default case condition should never be reached in this program.\n" );
                        return 1;

          }     /* switch( choice ) */

          /* Pause so the user can read the answer. */

          if ( choice != 17 )
          {
               printf( "Press Enter to continue... " );
               fflush( stdout );
               errno = 0;
               if ( fgets( buffer, 3, stdin ) == NULL )
               {
                    save_errno = errno;
                    if ( feof( stdin ) )
                    {
                         fprintf( stderr, "\nEnd of file on stdin.\n\n" );
                         return ( -1 );
                    }
                    fprintf( stderr, "\nFailed to read from stdin.\n" );
                    fprintf( stderr, "Error: %s.\n\n",
                             strerror( save_errno ) );
                    return ( -1 );
               }
               printf( "\n" );
          }    /* if ( choice != 17 ) */

     }    while( exit_loop == 0 );

     /* We're finally done. */

     printf( "Exiting.\n\n" );
     return 0;
}

/*

     Notice how a comment is used before every function.  It should describe
     what variables the function requires, what the function does, and what
     it returns.  I use the reserved keyword const when I pass something to
     a function or return a value from a function when I don't want it to be
     changed.  In a way, this sort of adds some level of documentation to
     the code.

*/

/*

     In this function we display the prompt and seek input from the user.

     The variable use_int will tell us whether the calling function wants
     an integer or double value returned.  The min_value specifies the
     minimum acceptable value for the input while the max_value specifies
     the maximum acceptable value for the input.  An integer input will be
     returned in choice while a double input will be returned in value.

     Returns 0 for success, 1 if the input buffer only
     contains a newline character, or -1 if an error occurs.

*/

/*

     The buffer variable in this function is declared as static so the
     compiler will put it somewhere else instead of on the stack with the
     other local variables every time this function is called.

*/

int get_input( const char *prompt, const double min_value,
               const double max_value, const int use_int, int *choice,
               double *value )
{
     static char buffer[ 130 ];
     int exit_loop, save_errno, size;

     exit_loop = 0;
     do
     {
          printf( "%s", prompt );
          fflush( stdout );

          /* We allow room for 128 characters in the answer plus 1 */
          /* for the newline and 1 for the null on the end of the  */
          /* input string.                                         */

          errno = 0;
          if ( fgets( buffer, 130, stdin ) == NULL )
          {
               save_errno = errno;

               /* If we've reached the end of the file on stdin then one */
               /* two things has happened.  Either the user entered the  */
               /* end of file character as their input or the user has   */
               /* redirected stdin on the command line so our program    */
               /* will read from a file instead of the user's keyboard.  */

               if ( feof( stdin ) )
               {
                    fprintf( stderr, "\nEnd of file on stdin.\n\n" );
                    exit( -1 );
               }
               fprintf( stderr, "\nFailed to read from stdin.\n" );
               fprintf( stderr, "Error: %s.\n\n", strerror( save_errno ) );
               exit( -1 );
          }

          size = strlen( buffer );
          if ( size < 2 )
          {
               if ( size == 1 && buffer[ 0 ] == '\n' )
               {
                    return 1; /* Reprint the prompt. */
               }

               /* Notice we don't need an else here. */

               return -1; /* Something else went wrong. */
          }

          size--;
          if ( buffer[ size ] == '\n' )
          {
               buffer[ size ] = 0;  /* Remove the newline character. */
          }

          /* sscanf() should remove any trailing letters for us */
          /* if a number of the correct type is entered first.  */

          if ( use_int )
          {

               /* Note that choice is already a pointer so we don't need */
               /* to use the & symbol to pass an address to sscanf().    */

               if ( sscanf( buffer, "%d", choice ) != 1 )
               {

                    /* Trying to fit each line in less   */
                    /* than 80 columns can be difficult. */

                    fprintf( stderr,
                             "\nSorry, only integer numbers are allowed.  "
                           );
                    fprintf( stderr,
                             "Please try again.\n\n" );
               }
               else
               {

                    /* Here minimum and maximum values are doubles */
                    /* and we need to convert them to integers so  */
                    /* we can compare them to choice.              */

                    /* It can be debated if a couple of local variables */
                    /* should be used instead of calling trunc() for    */
                    /* min_value and max_value twice.                   */


                    if ( *choice < ( int )trunc( min_value ) ||
                         *choice > ( int )trunc( max_value ) )
                    {
                         fprintf( stderr,
                                  "\nSorry, that value is out of range.\n"
                                );
                         fprintf( stderr,
                                  "The number must be between %d and %d ",
                                  ( int )trunc( min_value ),
                                  ( int )trunc( max_value ) );
                         fprintf( stderr, "inclusive.\n\n" );
                    }
                    else /* We found something we can use so we're done. */
                    {
                         exit_loop = 1;
                         *value = 0.0;
                    }
               }    /* if ( sscanf() ) */
          }
          else /* use_int == 0 */
          {

               /* Note that value is already a pointer so we don't need */
               /* to use the & symbol to pass an address to sscanf().   */

               /* Use %lG with sscanf() to read a double data type. */

               if ( sscanf( buffer, "%lG", value ) != 1 )
               {

                    /* Trying to fit each line in less   */
                    /* than 80 columns can be difficult. */

                    fprintf( stderr,
                             "\nSorry, only real numbers are allowed.  "
                           );
                    fprintf( stderr,
                             "Please try again.\n\n" );
               }
               else
               {
                    if ( *value < min_value || *value > max_value )
                    {
                         fprintf( stderr,
                                  "\nSorry, that value is out of range.\n"
                                );
                         fprintf( stderr,
                                  "The number must be between " );
                         fprintf( stderr,
                                  "%.15lG and %.15lG inclusive.\n\n",
                                  min_value, max_value );
                    }
                    else /* We found something we can use so we're done. */
                    {
                         exit_loop = 1;
                         *choice = 0;
                    }
               }    /* if ( sscanf() ) */
          }    /* if ( use_int ) */
     }    while( exit_loop == 0 );
     return 0;
}

/*

     In this function we list the program's menu options.

     It does not accept any variables, nor does it use
     any local variables, nor does it return anything.

*/

void list_menu( void )
{
     /* Clear the screen. */

     printf( "%c[1;1H%c[2J", 27, 27 );

     /* Print the menu. */

     printf( "Please choose from one of the following options:\n\n" );

     /* We can escape the newline in the source  */
     /* code to make it easier to type the menu. */

     printf( "\
 1) Convert from inches to centimeters.\n\
 2) Convert from centimeters to inches.\n\
 3) Convert from feet to meters.\n\
 4) Convert from meters to feet.\n\
 5) Convert from miles to kilometers.\n\
 6) Convert from kilometers to miles.\n\
 7) Convert from Fahrenheit to Celsius.\n\
 8) Convert from Celsius to Fahrenheit.\n\
 9) Convert from pounds to kilograms.\n\
10) Convert from kilograms to pounds.\n\
11) Convert from ounces to grams.\n\
12) Convert from grams to ounces.\n\
13) Convert from fluid ounces to milliliters.\n\
14) Convert from milliliters to fluid ounces.\n\
15) Convert from US gallons to liters.\n\
16) Convert from liters to US gallons.\n\
17) Exit.\n\n" );
     return;
}

/*

     In this function we display the answer requested by the user.

*/

void show_answer( const double answer, const char *units )
{

     /* Use %lG with printf() to print a double data type.         */
     /* In this case we are allowing up to 15 digits of precision. */

     printf( "\nAnswer: %.15lG %s.\n\n", answer, units );
     return;
}

/*

     In this function we convert inches to
     centimeters and return centimeters.

*/

double inches_to_centimeters( const double inches )
{
     return ( inches * 2.54 );
}

/*

     In this function we convert centimeters to inches and return inches.

*/

double centimeters_to_inches( const double centimeters )
{
     return ( centimeters / 2.54 );
}

/*

     In this function we convert feet to meters and return meters.

*/

double feet_to_meters( const double feet )
{
     return ( feet * 0.3048 );
}

/*

     In this function we convert from meters to feet and return feet.

*/

double meters_to_feet( const double meters )
{
     return ( meters / 0.3048 );
}

/*

     In this function we convert from miles
     to kilometers and return kilometers.

     Should we let the program do the math or should we find
     and use a single value?  I guess that can be open for debate.

     I think doing it this way shows the process.  If this function
     was called from a loop of some kind then we should use a single
     value instead of running the math like this over and over again.

*/

double miles_to_kilometers( const double miles )
{
     return ( miles * 5280.0 * 0.3048 / 1000.0 );
}

/*

     In this function we convert from kilometers to miles and return miles.

*/

double kilometers_to_miles( const double kilometers )
{
     return ( kilometers * 1000.0 / 0.3048 / 5280.0 );
}

/*

     In this function we convert from Fahrenheit
     to Celsius and return degrees Celsius.

*/

double fahrenheit_to_celsius( const double fahrenheit )
{
     return ( ( fahrenheit - 32.0 ) / 1.8 );
}

/*

     In this function we convert from Celsius to
     Fahrenheit and return degrees Fahrenheit.

*/

double celsius_to_fahrenheit( const double celsius )
{
     return ( ( celsius * 1.8 ) + 32.0 );
}

/*

     In this function we convert from pounds
     to kilograms and return kilograms.

*/

double pounds_to_kilograms( const double pounds )
{
     return ( pounds * 0.4535924 );
}

/*

     In this function we convert kilograms to pounds and return pounds.

*/

double kilograms_to_pounds( const double kilograms )
{

     return ( kilograms / 0.4535924 );
}

/*

     In this function we convert ounces to grams and return grams.

*/

double ounces_to_grams( const double ounces )
{
     return ( ounces / 16.0 * 0.4535924 * 1000.0 );
}

/*

     In this function we convert grams to ounces and return ounces.

*/

double grams_to_ounces( const double grams )
{
     return ( grams / 1000.0 / 0.4535924 * 16.0 );
}

/*

     In this function we convert fluid ounces
     to milliliters and return milliliters.

*/

double fluid_ounces_to_milliliters( const double fluid_ounces )
{
     return ( fluid_ounces / 128.0 * 3.785412 * 1000.0 );
}

/*

     In this function we convert milliliters
     to fluid ounces and return fluid ounces.

*/

double milliliters_to_fluid_ounces( const double milliliters )
{
     return ( milliliters / 1000.0 / 3.785412 * 128.0 );
}

/*

     In this function we convert US gallons to liters and return liters.

*/

double usgallons_to_liters( const double usgallons )
{
     return ( usgallons * 3.785412 );
}

/*

     In this function we convert liters to US gallons and return US gallons.

*/

double liters_to_usgallons( const double liters )
{
     return ( liters / 3.785412 );
}

/* Now that we've reached the end of the file we let the reader know. */

/* EOF convert.c */
