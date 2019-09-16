/*

     use_getnameinfo.c
     Uses getnameinfo(3).
     Written by Matthew Campbell on Saturday March 2, 2019.

*/

#define _POSIX_SOURCE

#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

int read_stdin( char *buffer, const int length, const char *prompt, const int reprompt );

/*

int getnameinfo( const struct sockaddr *sa, socklen_t salen,
                 char *host, size_t hostlen, char *serv, size_t servlen,
                 int flags );

void freeaddrinfo( struct addrinfo *res );

const char *gai_strerror( int errcode );

typedef uint16_t in_port_t;
typedef uint32_t in_addr_t;

struct in_addr
{
     in_addr_t s_addr;
};

struct sockaddr_in
{
     sa_family_t    sin_family;
	    in_port_t      sin_port;
	    struct in_addr sin_addr;
	    uint8_t        sin_zero[8];
};

struct in6_addr
{
	    union
     {
	         	uint8_t  __s6_addr[16];
	         	uint16_t __s6_addr16[8];
	         	uint32_t __s6_addr32[4];
     	} __in6_union;
};

#define s6_addr   __in6_union.__s6_addr
#define s6_addr16 __in6_union.__s6_addr16
#define s6_addr32 __in6_union.__s6_addr32

struct sockaddr_in6
{
     sa_family_t     sin6_family;
     in_port_t       sin6_port;
     uint32_t        sin6_flowinfo;
     struct in6_addr sin6_addr;
     uint32_t        sin6_scope_id;
};

struct ipv6_mreq
{
     struct in6_addr ipv6mr_multiaddr;
     unsigned        ipv6mr_interface;
};

*/

int main( void )
{
     char buffer[ 128 ], *ptr;
     char host[ NI_MAXHOST ], serv[ NI_MAXSERV ];
     int choice, port, pos, ret, save_errno;
     struct sockaddr_in sa;
     struct sockaddr_in6 sa6;

     memset( host, 0, NI_MAXHOST );
     memset( serv, 0, NI_MAXSERV );

/*
     strcpy( host, "bing.com" );
*/

     printf( "\
Would type of numeric IP address would you like to look up?\n\n" );
     printf( "1) IPv4\n" );
     printf( "2) IPv6\n\n" );
     errno = 0;
     if ( read_stdin( buffer, 128, ">> ", 1 ) != 0 )
     {
          save_errno = errno;
          printf( "Something went wrong while reading your input.\n" );
          if ( save_errno != 0 )
          {
               printf( "Error: %s.\n", strerror( save_errno ) );
          }
          printf( "\nProgram failed.  Exiting.\n\n" );
          exit( EXIT_FAILURE );
     }
     printf( "\n" );

     if ( sscanf( buffer, "%d", &choice ) != 1 )
     {
          printf( "That is not a valid choice.  Exiting.\n\n" );
          exit( EXIT_FAILURE );
     }

     if ( choice != 1 && choice != 2 )
     {
          printf( "That is not a valid choice.  Exiting.\n\n" );
          exit( EXIT_FAILURE );
     }

     errno = 0;
     if ( read_stdin( buffer, 128,
          "What host would you like to find? >> ", 1 ) != 0 )
     {
          save_errno = errno;
          printf( "Something went wrong while reading your input.\n" );
          if ( save_errno != 0 )
          {
               printf( "Error: %s.\n", strerror( save_errno ) );
          }
          printf( "\nProgram failed.  Exiting.\n\n" );
          exit( EXIT_FAILURE );
     }
     printf( "\n" );

     if ( choice == 1 )
     {
          for( pos = strlen( buffer ) - 1; pos >= 0 &&
               buffer[ pos ] != ':'; pos-- );
          if ( pos >= 0 )
          {
               buffer[ pos ] = 0;
               pos++;
               ptr = &buffer[ pos ];
               if ( sscanf( ptr, "%d", &port ) != 1 )
               {
                    printf( "Invalid port number: %s\n\n", ptr );
                    printf( "Program failed.  Exiting.\n\n" );
                    exit( EXIT_FAILURE );
               }
          }
          else
          {
               port = 0;
          }
     }
     else
     {
          port = 0;
     }

     printf( "Using host: \"%s\" port: %d\n\n", buffer, port );

// bing.com 204.79.197.200
// bing.com 13.107.21.200
// bing.com 2620:1ec:c11::200

     if ( choice == 1 )
     {
          memset( &sa, 0, sizeof( struct sockaddr_in ) );

          sa.sin_family = AF_INET;
          sa.sin_port = port;

          errno = 0;
          ret = inet_pton( AF_INET, buffer, &sa.sin_addr.s_addr );
          if ( ret != 1 )
          {
               save_errno = errno;
               printf( "\
Something went wrong when calling inet_pton(3).\n" );
               if ( ret == 0 )
               {
                    printf( "\
The specified host is not a valid network \
address for a numeric IPv4 address.\n" );
               }
               if ( save_errno != 0 )
               {
                    printf( "Error: %s.\n", strerror( save_errno ) );
               }
               printf( "\nProgram failed.  Exiting.\n\n" );
               exit( EXIT_FAILURE );
          }

          errno = 0;
          ret = getnameinfo( &sa, sizeof( struct sockaddr_in ),
                             host, NI_MAXHOST, serv, NI_MAXSERV,
                             NI_NAMEREQD );
          save_errno = errno;
          if ( ret != 0 )
          {
               printf( "\
Something went wrong when calling getnameinfo(3).\n" );
               if ( save_errno != 0 )
               {
                    printf( "Error: %s.\n", strerror( save_errno ) );
               }
               printf( "Error message: %s. (%d)\n", gai_strerror( ret ),
                       ret );
               printf( "\nProgram failed.  Exiting.\n\n" );
               exit( EXIT_FAILURE );
          }
     }
     else  /* choice == 2 */
     {
          memset( &sa6, 0, sizeof( struct sockaddr_in6 ) );

          sa6.sin6_family = AF_INET6;
          sa6.sin6_port = port;

          errno = 0;
          ret = inet_pton( AF_INET6, buffer, &sa6.sin6_addr.s6_addr );
          if ( ret != 1 )
          {
               save_errno = errno;
               printf( "\
Something went wrong when calling inet_pton(3).\n" );
               if ( ret == 0 )
               {
                    printf( "\
The specified host is not a valid network \
address for a numeric IPv6 address.\n" );
               }
               if ( save_errno != 0 )
               {
                    printf( "Error: %s.\n", strerror( save_errno ) );
               }
               printf( "\nProgram failed.  Exiting.\n\n" );
               exit( EXIT_FAILURE );
          }

          errno = 0;
          ret = getnameinfo( &sa6,
                             sizeof( struct sockaddr_in6 ),
                             host, NI_MAXHOST, serv, NI_MAXSERV,
                             NI_NAMEREQD );
          save_errno = errno;
          if ( ret != 0 )
          {
               printf( "\
Something went wrong when calling getnameinfo(3).\n" );
               if ( save_errno != 0 )
               {
                    printf( "Error: %s.\n", strerror( save_errno ) );
               }
               printf( "Error message: %s. (%d)\n", gai_strerror( ret ),
                       ret );
               printf( "\nProgram failed.  Exiting.\n\n" );
               exit( EXIT_FAILURE );
          }
     }

     printf( "Host: \"%s\", Service: \"%s\"\n\n", host, serv );

     printf( "Successful exit.\n\n" );
     exit( EXIT_SUCCESS );
}

/* EOF use_getnameinfo.c */
