#include "redpeter.h"

char device[] = "/dev/cu.usbmodem141301";
char host[] = "localhost";
char port[] = "6379";

int main(int argc, const char *argv[]) {

  printf("Starting serial socket bridge...\n\n");

  int dev_fd = open_device(device);
  int sock_fd = open_socket(host, port);
  printf("Done!\n\n");

  write(dev_fd, "+++", 3);

  while (1) read_and_write(dev_fd, sock_fd);

  return 0;
}

int open_device(char *device) {
  
  int fd = open(device, O_RDWR | O_NOCTTY);
  if (fd == -1) {
    fprintf(stderr, "Unable to open %s\n", device);
    exit(1);
  }
  
  printf("Connected to %s\n", device);

  return fd;
}

int open_socket(char *host, char *port) {

  int fd = socket(AF_INET, SOCK_STREAM, 0);

  struct addrinfo *addr_info;

  lookup_host(host, port, &addr_info);

  int status = connect(fd, addr_info->ai_addr, addr_info->ai_addrlen);
  if (status == -1) {
    fprintf(stderr, "Error connecting socket to %s:%s\n", host, port);
    exit(2);
  }

  freeaddrinfo(addr_info);

  printf("Connected to %s:%s\n", host, port);

  return fd;
}

void lookup_host(char *host, char *port, struct addrinfo **addr_info) {

  struct addrinfo hints;

  memset(&hints, 0, sizeof (struct addrinfo));
  hints.ai_family = AF_INET;        // IPv4 only
  hints.ai_socktype = SOCK_STREAM;  // TCP

  int status = getaddrinfo(host, port, &hints, addr_info);
  if (status != 0) {
    fprintf(stderr, "Error looking up host for %s:%s\n", host, port);
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
    exit(3);
  }
}

void read_and_write(int dev_fd, int sock_fd) {

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
    exit(4);
  }
  
  if (select_result > 0) {

    char buffer[128];
    int bytes_read;

    if (FD_ISSET(dev_fd, &set)) {
      printf("Device: ");
      bytes_read = read_from_descriptor(dev_fd, buffer, 128);

      write(sock_fd, &buffer, bytes_read);
    }

    if (FD_ISSET(sock_fd, &set)) {
      printf("Socket: ");
      bytes_read = read_from_descriptor(sock_fd, buffer, 128);
    }
  }
}

int read_from_descriptor(int fd, char *buffer, int size) {

  int bytes_read = read(fd, buffer, size);
  if (bytes_read > 0) {
    print_chars(buffer, bytes_read);
    printf("\n");
  } else {
    printf("No bytes\n");
  }

  return bytes_read;
}

void print_chars(char* bytes, int size) {
  for (int i = 0; i < size; i++) {
    char byte = bytes[i];
    if (iscntrl(byte)) {
      if (byte == 0) {
        printf("<NULL>");
      } else if (byte == 8) {
        printf("<BACK>");
      } else if (byte == 9) {
        printf("<TAB>");
      } else if (byte == 10) {
        printf("<LF>");
      } else if (byte == 11) {
        printf("<VTAB>");
      } else if (byte == 12) {
        printf("<FF>");
      } else if (byte == 13) {
        printf("<CR>");
      } else if (byte == 127) {
        printf("<DEL>");
      } else {
        printf("<%i>", byte);
      }
    } else {
      printf("%c", byte);
    }
  }
}
