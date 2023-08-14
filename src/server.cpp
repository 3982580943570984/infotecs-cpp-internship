#include <arpa/inet.h>  /* for inet_pton */
#include <iostream>     /* for cout */
#include <memory>       /* for unique_ptr */
#include <netinet/in.h> /* for sockaddr_in */
#include <string.h>     /* for memset() */
#include <sys/socket.h> /* for socket() */
#include <unistd.h>     /* for close() */

#define PORT 8080

int createTCPIpV4Socket(void);

sockaddr_in* createIPv4Address(const char* ip_address, uint16_t port);

int main() {
  // Creating socket
  int socket_fd = createTCPIpV4Socket();
  if (socket_fd == -1) {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }

  // Setting socket to reuse address
  sockaddr_in* address = createIPv4Address("127.0.0.1", 80);

  int addrlen = sizeof(*address);

  // Bind socket to address
  if (bind(socket_fd, (sockaddr *)&address, addrlen) < 0) {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }

  // Listen for connections
  if (listen(socket_fd, 3) < 0) {
    perror("listen");
    exit(EXIT_FAILURE);
  }

  // Accept connection
  int new_socket =
      accept(socket_fd, (sockaddr *)&address, (socklen_t *)&addrlen);
  if (new_socket < 0) {
    perror("accept");
    exit(EXIT_FAILURE);
  }

  // Processing received data
  char buffer[1024] = {0};
  while (true) {
    read(new_socket, buffer, 1024);
    std::cout << "Buffer data: " << buffer << std::endl;

    int len = strlen(buffer);
    int buffer_value = std::stoi(buffer);
    std::cout << "Data length: " << len << std::endl;
    if (len > 2 && buffer_value % 32 == 0) {
      std::cout << "Data is correct" << std::endl;
    } else {
      std::cout << "Error: Data is incorrect" << std::endl;
    }
  }

  return 0;
}

// AF_INET = IPv4 (Address Family)
// SOCK_STREAM = TCP (Socket Type)
int createTCPIpV4Socket(void) { return socket(AF_INET, SOCK_STREAM, 0); }

// rewrite 
// use std::unique_ptr
sockaddr_in* createIPv4Address(const char* ip_address, uint16_t port) {
  sockaddr_in* address = new sockaddr_in;
  address->sin_family = AF_INET;
  address->sin_port = htons(port); // htons = host to network short
  inet_pton(AF_INET, ip_address, &(address->sin_addr.s_addr));
  return address;
}