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

    do {

      printf("Top of inner loop\n");

      fd_set set;
      struct timeval timeout;

      timeout.tv_sec = 0; timeout.tv_usec = 100;

      int select_result = select(sock_fd + 1, &set, NULL, NULL, &timeout);
      if (select_result == - 1) {
        perror("select");
        exit(1);
      } else if (select_result == 0) {
          printf("No bytes availabe from socket\n");
          break;
      } else {

        sock_bytes_read = read(sock_fd, &buffer, 128);
        if (sock_bytes_read > 0) {
          printf("%i bytes from socket\n", sock_bytes_read);
          fwrite(buffer, sizeof(char), sock_bytes_read, stdout);
          printf("\n");
        } else {
          printf("No bytes read from socket\n");
        }

      }

    } while (sock_bytes_read > 0);

  } while (dev_bytes_read > 0);

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
  
  status = lookup_host(host, port, addr_info);
  if (status != 0) {
    fprintf(stderr, "Error looking up host for %s:%s\n", host, port);
    return status;
  }

  status = connect(*fd, addr_info->ai_addr, addr_info->ai_addrlen);
  if (status == -1) {
    fprintf(stderr, "Error connecting socket to %s:%s\n", host, port);
    return status;
  }

  printf("Connected to %s:%s\n", host, port);

  return 0;
}

int lookup_host(char *host, char *port, struct addrinfo *addr_info) {

  struct addrinfo hints;

  memset(&hints, 0, sizeof (struct addrinfo));
  hints.ai_family = AF_INET;        // IPv4 only
  hints.ai_socktype = SOCK_STREAM;  // TCP

  int status = getaddrinfo(host, port, &hints, &addr_info);
  if (status != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
    return status;
  }

  return 0;
}
