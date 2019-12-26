#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h> 
#include <sys/types.h>
#include <netdb.h>
#include <ctype.h>


int open_device(char* device);
int open_socket(char *host, char *port);
void lookup_host(char *host, char *port, struct addrinfo **addr_info);
void read_and_write(int dev_fd, int sock_fd);
int read_from_descriptor(int fd, char* buffer, int size);
void print_chars(char* bytes, int size);
