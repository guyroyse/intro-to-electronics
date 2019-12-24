#include "serial_socket_bridge.h"

char device[] = "/dev/cu.usbmodem141301";
char host[] = "localhost";
char port[] = "6379";

int main(int argc, const char *argv[]) {

  printf("Starting serial socket bridge...\n\n");

  int status;
  int dev_fd, sock_fd;
  
  status = open_device(device, &dev_fd);
  if (status) exit(1);

  status = open_socket(host, port, &sock_fd);
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

  int status;

  char buffer[128];
  int dev_bytes_read, sock_bytes_read;

  do {

    printf("Top of loop\n");

    dev_bytes_read = read(dev_fd, &buffer, 128);
    if (dev_bytes_read > 0) {
      printf("%i bytes from device\n", dev_bytes_read);
      fwrite(buffer, sizeof(char), dev_bytes_read, stdout);
      printf("\n");
    } else {
      printf("No bytes from device\n");
    }

    write(sock_fd, &buffer, dev_bytes_read);

    status = read_all_from_socket(sock_fd);

  } while (dev_bytes_read > 0);

  return 0;
}

int read_all_from_socket(int fd) {
  while (bytes_at_socket(fd)) {
    printf("> Top of socket read loop\n");
    int status = read_from_socket(fd);
  }

  return 0;
}

int bytes_at_socket(int fd) {

  fd_set set;

  struct timeval timeout;
  timeout.tv_sec = 0;
  timeout.tv_usec = 0;

  int select_result = select(fd + 1, &set, NULL, NULL, &timeout);
  if (select_result == - 1) {
    fprintf(stderr, "Error in select for %i", fd);
    return - 1;
  }
  
  if (select_result == 0) {
    printf("No bytes availabe from socket\n");
  }

  return select_result;

}

int read_from_socket(int fd) {

  char buffer[128];
  int bytes_read;

  bytes_read = read(fd, &buffer, 128);
  if (bytes_read > 0) {
    printf("%i bytes from socket\n", bytes_read);
    fwrite(buffer, sizeof(char), bytes_read, stdout);
    printf("\n");
  } else {
    printf("No bytes read from socket\n");
  }

  return 0;

}
