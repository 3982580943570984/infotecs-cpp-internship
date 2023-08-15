#include "./utils.h"

// AF_INET = IPv4 (Address Family)
// SOCK_STREAM = TCP (Socket Type)
int createTCPIPv4Socket(void) {
  int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

  if (socket_fd == -1) {
    std::cerr << "Error: Failed to create socket" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  return socket_fd;
}

std::unique_ptr<sockaddr_in> createIPv4Address(const char *ip_address,
                                               uint16_t port) {
  auto address = std::make_unique<sockaddr_in>();
  address->sin_family = AF_INET;
  address->sin_port = htons(port);

  if (inet_pton(AF_INET, ip_address, &(address->sin_addr)) == -1) {
    std::cerr
        << "Error: Failed to convert IP address from presentation to network "
           "format"
        << std::endl;
    std::exit(EXIT_FAILURE);
  }

  return address;
}