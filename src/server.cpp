#include <arpa/inet.h>  /* for inet_pton */
#include <iostream>     /* for std::cout */
#include <memory>       /* for std::unique_ptr */
#include <netinet/in.h> /* for sockaddr_in */
#include <string.h>     /* for memset() */
#include <sys/socket.h> /* for socket() */
#include <unistd.h>     /* for read() */

constexpr int PORT = 8080;
constexpr char IP_ADDRESS[] = "127.0.0.1";

int createTCPIpV4Socket(void);

std::unique_ptr<sockaddr_in> createIPv4Address(const char *ip_address,
                                               uint16_t port);

/*
  TODO:
    1. Add support for IPv6
    2. Add support for UDP
    3. Check for errors
    4. Move functions to a separate file
    5. Check for parasitic load
    6. Use classes
    7. Write modular tests
*/

int main() {
  // Creating socket
  int socket_fd = createTCPIpV4Socket();
  if (socket_fd == -1) {
    perror("Socket creation failed");
    exit(EXIT_FAILURE);
  }

  // Setting socket to reuse address
  auto serv_addr = createIPv4Address(IP_ADDRESS, 80);

  // Bind socket to address
  if (bind(socket_fd, (sockaddr *)serv_addr.get(), sizeof(*serv_addr)) == -1) {
    perror("Bind failed");
    exit(EXIT_FAILURE);
  }

  // Listen for connections
  if (listen(socket_fd, 1) == -1) {
    perror("Listen failed");
    exit(EXIT_FAILURE);
  }

  // Accept connection
  int addr_len = sizeof(*serv_addr);
  int new_socket = accept(socket_fd, (sockaddr *)&serv_addr, (socklen_t *)&addr_len);
  if (new_socket == -1) {
    perror("Accept failed");
    exit(EXIT_FAILURE);
  }

  // Processing received data
  char buffer[1024] = {0};
  while (true) {
    // check for error
    read(new_socket, buffer, 1024);
    std::cout << "Buffer data: " << buffer << std::endl;

    int len = strlen(buffer);
    std::cout << "Data length: " << len << std::endl;

    int buffer_value = std::stoi(buffer);
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
int createTCPIpV4Socket(void) { 
  // check for error
  return socket(AF_INET, SOCK_STREAM, 0);
}

std::unique_ptr<sockaddr_in> createIPv4Address(const char *ip_address,
                                               uint16_t port) {
  std::unique_ptr<sockaddr_in> address = std::make_unique<sockaddr_in>();
  address->sin_family = AF_INET;
  address->sin_port = htons(port);

  // check for errors
  inet_pton(AF_INET, ip_address, &(address->sin_addr));
  return address;
}