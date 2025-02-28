/*toralize.c*/

#include "toralize.h"

Req *request(struct sockaddr_in *sock2) {

Req *req;

req = malloc(reqsize);  // Allocate memory for the SOCKS4 request

req->vn = 4;                        // SOCKS4 version

req->cd = 1;                        // CONNECT command

req->dstport = sock2->sin_port;     // Destination port (big-endian)

req->dstip = sock2->sin_addr.s_addr;// Destination IP (big-endian)

strncpy(req->userid, USERNAME, 8);  // Copy username (max 8 characters)

return req;  // Return the pointer to the request structure

}

/* Hijacking the original connect, making our own to redirect network to Tor*/

int connect(int s2, const struct sockaddr *sock2,

socklen_t addrlen){

int     s;

struct sockaddr_in sock;

Req *req;

Res *res;

char buf[ressize];

int success;

char tmp[512];

int (*p)(int , const struct sockaddr*, socklen_t );

/* predicate - property */

p = dlsym(RTLD_NEXT, "connect");//regular or original connect function

s = socket(AF_INET, SOCK_STREAM, 0);//here we initiilaized a new socket

if (s < 0) {

perror("socket");

return -1;

}

sock.sin_family = AF_INET;

sock.sin_port = htons(PROXYPORT);//we connected here to the proxy port 9050

sock.sin_addr.s_addr = inet_addr(PROXY);//set proxy IP address to localhost

if (p(s, (struct sockaddr *)&sock, sizeof(sock))){  // call original connect() to reach proxy

perror("connect");

return -1;

};

printf("Connected to proxy\n");

req = request((struct sockaddr_in *)sock2);  // Build a SOCKS4 request

write(s, req, reqsize);  // Send request to proxy

memset(buf, 0, ressize);  // Clear buffer

if (read(s, buf, ressize) < 1) {  // Read response from proxy

perror("read");

free(req);

close(s);

return -1;

}

res = (Res *)buf;  // Convert buffer to response struct

success = (res->cd == 90);  // Check if response code is 90 (Success)

if (!success) {

fprintf(stderr, "Unable to traverse the proxy, error code: %d\n", res->cd);

close(s);

free(req);

return -1;

}

/* WE ARE NOW AUTHORIZED TO USE THE PROXY*/

/* Redirect traffic through proxy */

printf("successfully connected through the proxy.\n ");

dup2(s, s2);

free(req);

return 0;

/*

./toralize 1.2.3.4 80

argv0      argv1  argv2

*/

}