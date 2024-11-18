/*
structs and Data Handling

socket desctriptor -> int

network byte orde is also known as big-endian byte order

struck sockaddr {
    unsigned short sa_family; // address family, AF_xxx
    char sa_data[14]; // 14 bytes of protocol address
};

struct sockaddr_in { // 'in' comes from internet -> it was created so that programmers more easily populate the sa_data field and then cast it to sockaddr
    short sin_family; // should be set to "AF_INET"
    unsigned short sin_port; // port number - network byte order !!
    struct in_addr sin_addr; // internet address - network byte order !!
    unsigned char sin_zero[8]; // same size as struch sockaddr, this needs to be set to 0 with memset()
};

// Internet address (a structure for historical reasons)
struct in_addr {
  unsigned long s_addr; // that's a 32-bit long, or 4 bytes
};


short types are 2 bytes - 16 bits
long types are 4 bytes - 32 bits

htons() -- host to network short
htonl() -- host to network long
ntohs() -- network to host short
ntohl() -- network to host long


ina.sin_addr.s_addr = inet_addr("10.12.110.57");  - below is an alternative which is more modern

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
struct sockaddr_in my_addr;

my_addr.sin_family = AF_INET;         // host byte order
my_addr.sin_port = htons(MYPORT);     // short, network byte order
inet_aton("10.12.110.57", &(my_addr.sin_addr));
memset(&(my_addr.sin_zero), '\0', 8); // zero the rest of the struct


for TCP - use send() and recv() functions
for UDP - use sendto() and recvfrom() functions
Obs: can also use send() and recv() for UDP, but you will need to connect() first


getpeername(int sockfd, struct sockaddr *addr, socklen_t *addrlen); - to see who is connected to the socket
gethostname(char *hostname, size_t size);


Macros for manipulating file descriptor sets
FD_ZERO(fd_set *set) -- clears a file descriptor set
FD_SET(int fd, fd_set *set) -- adds fd to the set
FD_CLR(int fd, fd_set *set) -- removes fd from the set
FD_ISSET(int fd, fd_set *set) -- tests to see if fd is in the set


*/