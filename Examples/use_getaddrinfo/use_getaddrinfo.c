/*

     use_getaddrinfo.c
     Uses getaddrinfo(3).
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
#include <netinet/in.h>

int read_stdin( char *buffer, const int length, const char *prompt, const int reprompt );

/*

struct addrinfo                     // For use with hints.
{
     int              ai_flags;     // AI_CANONNAME
     int              ai_family;    // AF_INET or AF_INET6
     int              ai_socktype;  // SOCK_STREAM or SOCK_DGRAM
     int              ai_protocol;  // Zero
     socklen_t        ai_addrlen;   // Zero
     struct sockaddr *ai_addr;      // Null
     char            *ai_canonname; // Null
     struct addrinfo *ai_next;      // Null
};

node is the host address name
service is the remote port number expressed as a string or a service name
node or service may be null, but not both

If hints.ai_flags includes the AI_CANNONNAME flag, then
the ai_canonname field of the first of the addrinfo structures in the
returned list is set to point to the official name of the host.

A pointer to the socket address is placed in the ai_addr field, and the
length of the socket address, in bytes, is placed in the ai_addrlen field.

int getaddrinfo( const char *node, const char *service,
                 const struct addrinfo *hints, struct addrinfo **res );

void freeaddrinfo( struct addrinfo *res );

const char *gai_strerror( int errcode );

struct sockaddr
{
     sa_family_t sa_family;
     char        sa_data[14];
};

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
     char buffer[ 128 ], node_name[ 128 ], service_name[ 16 ], *str_ptr;
     int count, pos, ret, save_errno, size;
     struct addrinfo hints, *list_ptr, *res;
     struct sockaddr_in *addr_ptr;
     struct sockaddr_in6 *addr6_ptr;
     unsigned int num;

     union
     {
          unsigned int num;
          char bytes[ 4 ];
     } box;

     errno = 0;
     if ( read_stdin( buffer, 128,
          "What host would you like to try? >> ", 1 ) != 0 )
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

     size = strlen( buffer );
     for( pos = ( size - 1 ); pos >= 0 && buffer[ pos ] != ':'; pos-- );
     if ( pos >= 0 )
     {
          str_ptr = &buffer[ ( pos + 1 ) ];
          if ( sscanf( str_ptr, "%u", &num ) == 1 )
          {
               strncpy( service_name, str_ptr, 15 );
               buffer[ pos ] = 0;
          }
     }
     else
     {
          strncpy( service_name, "0", 2 );
     }
     strncpy( node_name, buffer, 127 );
     memset( &hints, 0, sizeof( struct addrinfo ) );
     hints.ai_flags = AI_CANONNAME | AI_PASSIVE;
     hints.ai_family = AF_UNSPEC;
     hints.ai_socktype = 0;

     printf( "Using node_name: \"%s\", port: \"%s\"\n\n", node_name,
             service_name );

     errno = 0;
     ret = getaddrinfo( node_name, service_name, &hints, &res );
     save_errno = errno;
     if ( ret != 0 )
     {
          printf( "Something went wrong when calling getaddrinfo(3).\n" );
          if ( save_errno != 0 )
          {
               printf( "Error: %s (%d).\n", strerror( save_errno ),
                       save_errno );
          }
          printf( "Error message: %s. (%d)\n", gai_strerror( ret ), ret );
          printf( "\nProgram failed.  Exiting.\n\n" );
          exit( EXIT_FAILURE );
     }

     list_ptr = res;
     pos = 1;
     while( list_ptr != NULL )
     {
          printf( "                List element: %d\n\n", pos );

          printf( "                    ai_flags: %d\n",
                  list_ptr->ai_flags );

          printf( "                   ai_family: " );
          if ( list_ptr->ai_family == AF_INET )
          {
               printf( "IPv4 (AF_INET)\n" );
               addr_ptr = ( struct sockaddr_in * )list_ptr->ai_addr;
          }
          else if ( list_ptr->ai_family == AF_INET6 )
          {
               printf( "IPv6 (AF_INET6)\n" );
               addr6_ptr = ( struct sockaddr_in6 * )list_ptr->ai_addr;
          }
          else
          {
               printf( "Unknown: %d\n", list_ptr->ai_family );
          }

          printf( "                 ai_socktype: " );

          switch( list_ptr->ai_socktype )
          {
               case SOCK_STREAM    : printf( "Stream\n" );
                                     break;
               case SOCK_DGRAM     : printf( "Datagram\n" );
                                     break;
               case SOCK_RAW       : printf( "Raw\n" );
                                     break;
               case SOCK_RDM       : printf( "RDM\n" );
                                     break;
               case SOCK_SEQPACKET : printf( "Sequential Packet\n" );
                                     break;
               case SOCK_PACKET    : printf( "Packet\n" );
                                     break;
                           default : printf( "Unknown: %d\n",
                                             list_ptr->ai_family );
                                     break;
          }

          printf( "                  ai_addrlen: %d\n",
                  list_ptr->ai_addrlen );

          if ( list_ptr->ai_family == AF_INET )
          {
               printf( "         ai_addr->sin_family: " );

               switch( addr_ptr->sin_family )
               {
                    case SOCK_STREAM    : printf( "Stream\n" );
                                          break;
                    case SOCK_DGRAM     : printf( "Datagram\n" );
                                          break;
                    case SOCK_RAW       : printf( "Raw\n" );
                                          break;
                    case SOCK_RDM       : printf( "RDM\n" );
                                          break;
                    case SOCK_SEQPACKET : printf( "Sequential Packet\n" );
                                          break;
                    case SOCK_PACKET    : printf( "Packet\n" );
                                          break;
                                default : printf( "Unknown: %d\n",
                                                  list_ptr->ai_family );
                                          break;
               }
          }
          else if ( list_ptr->ai_family == AF_INET6 )
          {
               printf( "        ai_addr->sin6_family: " );

               switch( addr6_ptr->sin6_family )
               {
                    case SOCK_STREAM    : printf( "Stream\n" );
                                          break;
                    case SOCK_DGRAM     : printf( "Datagram\n" );
                                          break;
                    case SOCK_RAW       : printf( "Raw\n" );
                                          break;
                    case SOCK_RDM       : printf( "RDM\n" );
                                          break;
                    case SOCK_SEQPACKET : printf( "Sequential Packet\n" );
                                          break;
                    case SOCK_PACKET    : printf( "Packet\n" );
                                          break;
                                default : printf( "Unknown: %d\n",
                                                  list_ptr->ai_family );
                                          break;
               }
          }

          if ( list_ptr->ai_family == AF_INET )
          {
               printf( "    ai_addr->sin_addr.s_addr: " );
               box.num = addr_ptr->sin_addr.s_addr;
               for( count = 0; count < 4; count++ )
               {
                    printf( "%02X",
                            ( unsigned char )box.bytes[ count ] );
                    if ( count < 3 )
                    {
                         printf( "%c", 32 );
                    }
               }
               printf( "\n" );
          }
          else if ( list_ptr->ai_family == AF_INET6 )
          {
               printf( "  ai_addr->sin6_addr.s6_addr: " );
               for( count = 0; count < 16; count++ )
               {
                    printf( "%02X",
( unsigned char )addr6_ptr->sin6_addr.s6_addr[ count ] );
                    if ( count < 15 )
                    {
                         printf( "%c", 32 );
                    }
               }
               printf( "\n" );
          }

          if ( list_ptr->ai_family == AF_INET )
          {
               errno = 0;
               if ( inet_ntop( list_ptr->ai_family,
                               &( addr_ptr->sin_addr.s_addr ),
                               buffer, 128 ) == NULL )
               {
                    save_errno = errno;
                    printf( "\
Something went wrong while using inet_ntop(3).\n" );
                    if ( save_errno != 0 )
                    {
                         printf( "Error: %s.\n", strerror( save_errno ) );
                    }
                    printf( "\n" );
               }
               else
               {
                    printf( "                  IP Address: \"%s\"\n",
                            buffer );
               }
          }
          else if ( list_ptr->ai_family == AF_INET6 )
          {
               errno = 0;
               if ( inet_ntop( list_ptr->ai_family,
                               addr6_ptr->sin6_addr.s6_addr,
                               buffer, 128 ) == NULL )
               {
                    save_errno = errno;
                    printf( "\
Something went wrong while using inet_ntop(3).\n" );
                    if ( save_errno != 0 )
                    {
                         printf( "Error: %s.\n", strerror( save_errno ) );
                    }
                    printf( "\n" );
               }
               else
               {
                    printf( "                  IP Address: \"%s\"\n",
                            buffer );
               }
          }

          if ( list_ptr->ai_canonname != NULL )
          {
               printf( "                ai_canonname: \"%s\"\n\n",
                       list_ptr->ai_canonname );
          }
          else
          {
               printf( "                ai_canonname: NULL\n\n" );
          }

          list_ptr = list_ptr->ai_next;
          pos++;
     }

     freeaddrinfo( res );

     printf( "Successful exit.\n\n" );
     exit( EXIT_SUCCESS );
}

/* EOF use_addrinfo.c */
