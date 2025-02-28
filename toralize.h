/* toralize.h */
#include <stdio.h>      // Standard Input/Output functions
#include <stdlib.h>     // Standard Library Functions
#include <string.h>     // String Manipulation Functions
#include <unistd.h>     // POSIX API 
#include <sys/socket.h> // Socket API
#include <arpa/inet.h>  // Functions for IP address manipulations
#include <netinet/in.h> // Structures for internet domain addresses
#include <dlfcn.h>      // Dynamic Linking, used to hijack connect() function


#define PROXY       "127.0.0.1" // Local TOR Proxy Address
#define PROXYPORT   9050        // Default SOCKS4 proxy port for TOR
#define USERNAME "toraliz"      //Username for SOCKS4 authentication
#define reqsize sizeof(struct proxy_request)  // Size of SOCKS4 request struct
#define ressize sizeof(struct proxy_response) // Size of SOCKS4 response struct

typedef unsigned char int8;       // 1-byte 
typedef unsigned short int int16; // 2-bytes
typedef unsigned int int32;       // 4-bytes

/*
  SOCKS4 Request Format

                +----+----+----+----+----+----+----+----+----+----+....+----+
                | VN | CD | DSTPORT |      DSTIP        | USERID       |NULL|
                +----+----+----+----+----+----+----+----+----+----+....+----+
 # of bytes:	   1    1      2              4           variable       1

*/

struct proxy_request {
  int8 vn;           // SOCKS version number (4 for SOCKS4)
  int8 cd;           // Command code (1 = CONNECT, 2 = BIND)
  int16 dstport;     // Destination port (big-endian format)
  int32 dstip;       // Destination IP address (big-endian format)
  unsigned char userid[8];  // User ID (SOCKS4 requires a username)
};
typedef struct proxy_request Req; // Define an alias for easy reference


/*
  SOCKS4 Response Format
                +----+----+----+----+----+----+----+----+
                | VN | CD | DSTPORT |      DSTIP        |
                +----+----+----+----+----+----+----+----+
 # of bytes:	   1    1      2              4
*/


struct proxy_response {
  int8 vn;  // Response version (always 0 in SOCKS4)
  int8 cd;  // Response status code (90 = success, 91 = failure)
  int16 _;  // Unused (padding)
  int32 __; // Unused (padding)
};
typedef struct proxy_response Res; // Alias for easy reference


Req *request(struct sockaddr_in *);
int connect (int , const struct sockaddr*, socklen_t );
/*
  Purpose of this Header File

  1. TURN THE CLIENT INTO A LIB(SHARED LIBRARY) .SO
  2. TURN MAIN() INTO OUR OWN CONNECT()
  3. REPLACE REGULAR CONNECT()
  4. GRAP IP AND PORT FROM ORIG CONNECT()
  
*/