#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h> 
#include <sys/types.h>
#include <netdb.h>


int open_device(char* device, int *fd);
int open_socket(char *host, char *port, int *fd);
int lookup_host(char *host, char *port, struct addrinfo *addr_info);