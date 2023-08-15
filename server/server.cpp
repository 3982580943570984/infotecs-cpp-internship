#include "./server.h"

Server::Server() { memset(buffer, 0, sizeof(buffer)); }

Server::Server(std::string ip_address, int port)
    : ip_address(ip_address), port(port) {}

Server::~Server() {
  close(server_socket_fd);
  close(client_socket_fd);
}

void Server::createSocket() { server_socket_fd = createTCPIPv4Socket(); }

void Server::setReuseAddress() {
  server_address = createIPv4Address(ip_address.c_str(), port);
}

void Server::bindSocket() {
  if (bind(server_socket_fd, (sockaddr *)server_address.get(),
           sizeof(*server_address)) == -1) {
    perror("Bind failed");
    std::exit(EXIT_FAILURE);
  }
}

void Server::listenForConnections() {
  if (listen(server_socket_fd, 1) == -1) {
    perror("Listen failed");
    std::exit(EXIT_FAILURE);
  }
}

void Server::acceptConnection() {
  int serv_addr_len = sizeof(*server_address);
  client_socket_fd = accept(server_socket_fd, (sockaddr *)&server_address,
                            (socklen_t *)&serv_addr_len);
  if (client_socket_fd == -1) {
    perror("Accept failed");
    std::exit(EXIT_FAILURE);
  }
}

void Server::processReceivedData() {
  while (true) {
    int bytes_received =
        recv(client_socket_fd, buffer, sizeof(buffer), MSG_PEEK);
    if (bytes_received == -1) {
      perror("Error: Failed to receive data");
      std::exit(EXIT_FAILURE);
    } else if (bytes_received == 0) {
      std::cout << "Client disconnected" << std::endl;
      break;
    }

    if (read(client_socket_fd, buffer, sizeof(buffer)) == -1) {
      perror("Error: Failed to read data from client");
      std::exit(EXIT_FAILURE);
    }

    std::cout << "Buffer data: " << buffer << std::endl;

    int buffer_len = strlen(buffer);
    std::cout << "Buffer data length: " << buffer_len << std::endl;

    int buffer_value = 0;
    try {
      buffer_value = std::stoi(buffer);
      std::cout << "Buffer data as integer: " << buffer_value << std::endl;
    } catch (const std::invalid_argument &e) {
      std::cerr
          << "Error: Invalid argument. Failed to convert buffer to an integer."
          << std::endl;
      continue;
    } catch (const std::out_of_range &e) {
      std::cerr
          << "Error: Out of range. Failed to convert buffer to an integer."
          << std::endl;
      continue;
    }

    if (buffer_len > 2 && buffer_value % 32 == 0) {
      std::cout << "Data is correct" << std::endl;
    } else {
      std::cout << "Error: Data is incorrect (invalid length or mod)"
                << std::endl;
    }

    std::cout << std::endl;

    memset(buffer, 0, sizeof(buffer));
  }
}

void Server::start() {
  createSocket();
  setReuseAddress();
  bindSocket();
  listenForConnections();
  acceptConnection();
  processReceivedData();
}