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
int lookup_host(char *host, char *port, struct addrinfo **addr_info);
int start_read_write_loop(int dev_fd, int sock_fd);
int read_all_from_socket(int fd);
int bytes_at_socket(int fd);
int read_from_socket(int fd);