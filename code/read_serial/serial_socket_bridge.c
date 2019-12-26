#include "serial_socket_bridge.h"

char device[] = "/dev/cu.usbmodem141301";
char host[] = "localhost";
char port[] = "6379";

int main(int argc, const char *argv[]) {

  printf("Starting serial socket bridge...\n\n");

  int status;
  int dev_fd, sock_fd;
  
  status = open_socket(host, port, &sock_fd);
  if (status) exit(1);

  status = open_device(device, &dev_fd);
  if (status) exit(1);

  status = start_read_write_loop(dev_fd, sock_fd);
  if (status) exit(1);

  return 0;
}

int open_device(char *device, int *fd) {
  
  *fd = open(device, O_RDONLY);
  if (*fd == -1) {
    fprintf(stderr, "Unable to open %s\n", device);
    return -1;
  }
  
  printf("Connected to %s\n", device);

  return 0;
}

int open_socket(char *host, char *port, int *fd) {

  *fd = socket(AF_INET, SOCK_STREAM, 0);

  struct addrinfo *addr_info;

  int status;
  
  status = lookup_host(host, port, &addr_info);
  if (status != 0) {
    fprintf(stderr, "Error looking up host for %s:%s\n", host, port);
    return status;
  }

  status = connect(*fd, addr_info->ai_addr, addr_info->ai_addrlen);
  if (status == -1) {
    fprintf(stderr, "Error connecting socket to %s:%s\n", host, port);
    return status;
  }

  freeaddrinfo(addr_info);

  printf("Connected to %s:%s\n", host, port);

  return 0;
}

int lookup_host(char *host, char *port, struct addrinfo **addr_info) {

  struct addrinfo hints;

  memset(&hints, 0, sizeof (struct addrinfo));
  hints.ai_family = AF_INET;        // IPv4 only
  hints.ai_socktype = SOCK_STREAM;  // TCP

  int status = getaddrinfo(host, port, &hints, addr_info);
  if (status != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
    return status;
  }

  return 0;
}

int start_read_write_loop(int dev_fd, int sock_fd) {

  while (1) {

    struct timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;

    fd_set set;
    FD_ZERO(&set);
    FD_SET(dev_fd, &set);
    FD_SET(sock_fd, &set);

    int select_result = select(FD_SETSIZE, &set, NULL, NULL, &timeout);
    if (select_result == -1) {
      printf("Error in select.\n");
      return -1;
    } else if (select_result > 0) {
      printf("%i descriptors available to read\n\n", select_result);

      if (FD_ISSET(dev_fd, &set)) {

        printf("Device descriptor available to read\n");
        printf("-----------------------------------\n");

        char buffer[128];

        int dev_bytes_read = read(dev_fd, &buffer, 128);
        if (dev_bytes_read > 0) {
          printf("%i bytes from device\n", dev_bytes_read);
          fwrite(buffer, sizeof(char), dev_bytes_read, stdout);
          printf("\n");
          print_bytes(buffer, dev_bytes_read);
        } else {
          printf("No bytes from device\n");
        }

        printf("\n");

        write(sock_fd, &buffer, dev_bytes_read);

      }

      if (FD_ISSET(sock_fd, &set)) {
        printf("Socket descriptor available to read\n");
        printf("-----------------------------------\n");
        read_from_socket(sock_fd);
        printf("\n");
      }
    }
  }
}

int read_from_socket(int fd) {

  char buffer[128];
  int bytes_read;

  bytes_read = read(fd, &buffer, 128);
  if (bytes_read > 0) {
    printf("%i bytes from socket\n", bytes_read);
    fwrite(buffer, sizeof(char), bytes_read, stdout);
    printf("\n");
    print_bytes(buffer, bytes_read);
  } else {
    printf("No bytes read from socket\n");
  }

  return 0;

}

void print_bytes(char *bytes, int size) {
  for (int i = 0; i < size; i++) {
    printf( "%i ", bytes[i]);
  }
  printf("\n");
}
