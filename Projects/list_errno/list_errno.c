/*

     list_errno.c
     Shows the short codes, the numbers, and the errno strings.
     Written by Matthew Campbell on Wednesday November 28, 2018.

     These codes were copied from <bits/errno.h>.

*/

#include <errno.h>
#include <stdio.h>
#include <string.h>

char short_codes[ 134 ][ 16 ] = { \
"", "EPERM", "ENOENT", "ESRCH", "EINTR", "EIO", "ENXIO", "E2BIG",
"ENOEXEC", "EBADF", "ECHILD", "EAGAIN", "ENOMEM", "EACCES", "EFAULT",
"ENOTBLK", "EBUSY", "EEXIST", "EXDEV", "ENODEV", "ENOTDIR", "EISDIR",
"EINVAL", "ENFILE", "EMFILE", "ENOTTY", "ETXTBSY", "EFBIG", "ENOSPC",
"ESPIPE", "EROFS", "EMLINK", "EPIPE", "EDOM", "ERANGE", "EDEADLK",
"ENAMETOOLONG", "ENOLCK", "ENOSYS", "ENOTEMPTY", "ELOOP", "EWOULDBLOCK",
"ENOMSG", "EIDRM", "ECHRNG", "EL2NSYNC", "EL3HLT", "EL3RST", "ELNRNG",
"EUNATCH", "ENOCSI", "EL2HLT", "EBADE", "EBADR", "EXFULL", "ENOANO",
"EBADRQC", "EBADSLT", "EDEADLOCK", "EBFONT", "ENOSTR", "ENODATA",
"ETIME", "ENOSR", "ENONET", "ENOPKG", "EREMOTE", "ENOLINK", "EADV",
"ESRMNT", "ECOMM", "EPROTO", "EMULTIHOP", "EDOTDOT", "EBADMSG",
"EOVERFLOW", "ENOTUNIQ", "EBADFD", "EREMCHG", "ELIBACC", "ELIBBAD",
"ELIBSCN", "ELIBMAX", "ELIBEXEC", "EILSEQ", "ERESTART", "ESTRPIPE",
"EUSERS", "ENOTSOCK", "EDESTADDRREQ", "EMSGSIZE", "EPROTOTYPE",
"ENOPROTOOPT", "EPROTONOSUPPORT", "ESOCKTNOSUPPORT", "EOPNOTSUPP",
"EPFNOSUPPORT", "EAFNOSUPPORT", "EADDRINUSE", "EADDRNOTAVAIL",
"ENETDOWN", "ENETUNREACH", "ENETRESET", "ECONNABORTED", "ECONNRESET",
"ENOBUFS", "EISCONN", "ENOTCONN", "ESHUTDOWN", "ETOOMANYREFS",
"ETIMEDOUT", "ECONNREFUSED", "EHOSTDOWN", "EHOSTUNREACH", "EALREADY",
"EINPROGRESS", "ESTALE", "EUCLEAN", "ENOTNAM", "ENAVAIL", "EISNAM",
"EREMOTEIO", "EDQUOT", "ENOMEDIUM", "EMEDIUMTYPE", "ECANCELED",
"ENOKEY", "EKEYEXPIRED", "EKEYREVOKED", "EKEYREJECTED", "EOWNERDEAD",
"ENOTRECOVERABLE", "ERFKILL", "EHWPOISON" };

int main( void )
{
     int count;

     /* A successful return does not use a short code. */

     printf( "%3d: %s\n", 0, strerror( 0 ) );

     for( count = 1; count < 134; count++ )
     {
          /* Handle special cases first. */

          if ( count == 41 )
          {
               printf( "   : %s: EAGAIN (11)\n", short_codes[ count ] );
          }
          else if ( count == 58 )
          {
               printf( "   : %s: EDEADLK (35)\n", short_codes[ count ] );
          }
          else if ( count == 95 )
          {
               printf( "%3d: %s: %s\n", count, short_codes[ count ],
                       strerror( count ) );
               printf( "   : ENOTSUP: EOPNOTSUPP (95)\n" );
          }
          else
          {
               printf( "%3d: %s: %s\n", count, short_codes[ count ],
                       strerror( count ) );
          }
     }

     return 0;
}

/* EOF list_errno.c */
