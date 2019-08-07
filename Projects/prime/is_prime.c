/*

     This is the work horse function of this program.
     This is where we find out if the number we are
     testing is a prime number.  A prime number is a
     number that only has the factors of 1 and itself.

*/

int is_prime( int num )
{
     int limit;
     register int count;

     if ( num == 0 )
     {
          return 0;  /* Zero is not a prime number. */
     }

/*

     This code may be copied and used in someone
     else's program and if that be the case then
     we can't be sure about the input passed to
     this function.

*/

     if ( num < 0 )
     {

#ifdef NEGATIVE_IS_NOT_PRIME

          return 0;  /* Strict */

#endif

          num *= ( -1 );  /* Just flip it and use it anyway. */
     }

/*

     Some may want 1 listed with prime numbers.
     If they do they can #define LIST_ONE_AS_PRIME
     at the top of the program with the other definitions.

*/

#ifdef LIST_ONE_AS_PRIME

     if ( num == 1 )
     {
          return 1;
     }

#else

     if ( num == 1 )
     {
          return 0;
     }

#endif

/*

     Take care of an obvious case.

*/

     if ( num == 2 )
     {
          return 1;  /* 2 is a prime number. */
     }

/*

     Any even number greater than 2 is not a prime number because
     it is evenly divisible by 2 so we shouldn't waste our time with it.

*/

     if ( num % 2 == 0 )
     {
          return 0;
     }

/*

     At this point we know that the number is an odd number greater
     than 2.  The number cannot have any factors, other than itself,
     that are greater than the square root of the number that we are
     testing so it is pointless to search for them.  We will increment
     count by two steps so we don't search for even numbered factors.
     If the number we are testing is 3 then the for() loop won't even
     be used.

*/

     limit = ( int )sqrt( num );
     for ( count = 3; count <= limit; count += 2 )
     {
          if ( num % count == 0 )
          {
               return 0;  /* We found a factor so num */
          }               /* is not a prime number.   */
     }

/*

     We didn't find any factors so num is a prime number.

*/

     return 1;
}

/* EOF is_prime.c */

