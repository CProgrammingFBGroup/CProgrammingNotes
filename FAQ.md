# Group FAQ

# 1. What books or other resources should I use to learn C?
  
   You can find a list of resources [here](https://github.com/aleksandar-todorovic/awesome-c#learning-reference-and-tutorials)
   
   You can also legally download a free ebook [here](https://goalkicker.com/CBook)
 
# 2. What compiler is best?

   You can find a list of Free and Open Source Compilers [here](https://www.facebook.com/notes/c-programming/a-beginners-guide-to-using-freeopen-source-software-development-tools-and-enviro/676277512460549/) or [here](https://github.com/aleksandar-todorovic/awesome-c#compilers)
  
# 3. How do I use random numbers in C?
  
   You need to 
    
   1. Initialize the seed. 
    The seed is a number that multiplies with a couple random numbers (internally). You should use time(NULL) as the input parameter of the srand function (seed random). 
   2. Initialize the random variable with the rand() function. 
     If you need a range (numbers 0-20 or whatever) use the modulus ('%') 
     operator to generate your number. Example:
  
```c
#include <time.h>
#include <stdlib.h>

srand(time(NULL));   // should only be called once
int foo = rand();      // returns a pseudo-random integer between 0 and RAND_MAX
int bar = rand() % 55; //returns a pseudo-random integer between 0 and 54
int baz = (rand() % 55) + 1; // returns a psuedo-random integer between 1 and 55
```


# 4. Why should I not use Turbo-C/Turbo-C++
   
   You should never use this IDE because it is outdated. It uses header files that are deprecated , 
and havent been used since the mid-90's. These headers include:
   
   1. dos.h
   2. conio.h
   3. graphics.h
   4. iostream.h (for C++)
   
   Those header files are never used in modern C runtime libraries. As a matter of fact, 
C++ requires you to use iostream (without the .h), the iostream.h will never compile 
correctly (on modern compilers). This makes code highly unportable (as a matter of fact, 
it sets up an emulated dos enviornment for you to actually run your programs). This 
makes the user spend twice the time learning things, when they could have learned it 
correctly the first time.

# 5. How do I print a prompt and then read input on the same line?

Print the prompt using printf() and use fflush() afterwards.  It is necessary to call fflush() because printf() uses buffered I/O which will only write the data in the buffer when enough data has accumulated or when a newline character is encountered.

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main( void )
{
     char str[ 101 ];
     int loop, size;

     printf( "Please enter your choice: >> " );
     fflush( stdout );
     do
     {
          loop = 0;
          if ( fgets( str, 100, stdin ) == NULL )
          {
               printf( "Sorry, something went wrong.\n" );
               exit( EXIT_FAILURE );
          }
          if ( str[ 0 ] == 0 )
          {
               printf( "Sorry, no input received.\n" );
               exit( EXIT_FAILURE );
          }
          size = strlen( str );
          if ( size == 1 && str[ 0 ] == '\n' )
          {
          
               /* Silently loop around again. */
               
               loop = 1;
          }
          else
          {
               size--;
               if ( str[ size ] == '\n' )
               {
          
                    /* Remove the newline. */
               
                    str[ size ] = 0;
               }
          }
     }    while( loop == 1 );
     
     /* Now you may use the input... */
     
     /* Later on... */
     
     exit( EXIT_SUCCESS );
}
```

The use of the loop variable allows the do-while() loop to silently loop around for a second pass if a stray newline character is left in the stdin buffer.

# 6. How do I use scanf() to read multiple words instead of just the first word in a character string?

You can use scanf( "%[^\n]", str );  If scanf() returns right away without giving you a chance to enter any input then there may be a newline character in the input buffer which can be cleared by using ch = ( char )fgetc( stdin ); before calling scanf().

An alternative option to using scanf() may be found in examples/read_stdin/ which uses fgets().

# 7. How can I pass arguments to my C program on the command line at runtime?

Use the argument count and argument values variables in main().

```c
#include <stdio.h>
#include <stdlib.h>

int main( int argc, char **argv )
{
     int count;
     
     if ( argc < 2 )
     {
          printf( "No command line arguments supplied.\n" );
          exit( EXIT_FAILURE );
     }
     printf( "You used:\n\n" );
     for( count = 1; count < argc; count++ )
     {
          printf( "argv[ %d ]: \"%s\"\n", count, argv[ count ] );
     }
     return 0;
}
```

If the program is run without any command line arguments then argc will be 1.  The name of the program will be stored in argv[ 0 ].  If you use three command line arguments argc will be four, one for the name of the program, plus three because of the three command line arguments.  The valid array elements in argv will run from zero to ( argc - 1 ).  The variable argv is a two dimensional character array, also known as an array of strings.  If you ran the code listed above with the name show_args and the argument John then you could access the letter J with argv[ 1 ][ 0 ].

Example: show_args John

Now argc will equal two.  argv[ 0 ] will hold "show_args" and argv[ 1 ] will hold "John".  argv[ 1 ][ 2 ] will hold 'h'.

# 8. How can I get the compiler to store my variable in a register in the CPU?

You can define an int as: register int count;
This is a reserved keyword that tells the compiler to try to use a register when using the variable 'count'.

It does not guarantee that the variable will use a register, but it can increase the likelihood.  Good compilers should optimize the output code that they produce to do things like this anyway.

# 9. How can I clear the screen in C?

You can use: printf( "%c[1;1H%c[2J", 27, 27 );

You might want to write that into a function.

# 10. How can I reverse the order of the bytes in a string?

See examples/strrev/strrev.c [here](https://github.com/CProgrammingFBGroup/CProgrammingNotes/blob/master/examples/strrev/strrev.c)

## Another-C-FAQ

   [C-FAQ](http://c-faq.com/questions.html)
