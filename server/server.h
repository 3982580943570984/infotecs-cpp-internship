#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <memory>

#include <string.h>

#include <unistd.h>

#include "../other/utils.h"

class Server {
public:
  Server();
  Server(std::string ip_address, int port);
  ~Server();

  void start();

private:
  int server_socket_fd;
  int client_socket_fd;
  std::unique_ptr<sockaddr_in> server_address;
  std::string ip_address;
  int port;
  char buffer[1024];

  void createSocket();
  void setReuseAddress();
  void bindSocket();
  void listenForConnections();
  void acceptConnection();
  void processReceivedData();
};

#endif /* SERVER_H */