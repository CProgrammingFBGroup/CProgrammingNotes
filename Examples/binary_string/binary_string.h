/*

     binary_string.h
     Prototypes the functions in binary_string.c.
     Written by Matthew Campbell.

*/

#ifndef _BINARY_STRING_H
#define _BINARY_STRING_H

void binary_string_show_leading_zeros( void );
void binary_string_suppress_leading_zeros( void );

const char *int8_to_binary_string( const char num );
const char *int16_to_binary_string( const short int num );
const char *int32_to_binary_string( const long int num );
const char *int64_to_binary_string( const long long int num );

int binary_string_to_int8( const char *str, char *num );
int binary_string_to_int16( const char *str, short int *num );
int binary_string_to_int32( const char *str, long int *num );
int binary_string_to_int64( const char *str, long long int *num );

#endif  /* _BINARY_STRING_H */

/* EOF binary_string.h */
