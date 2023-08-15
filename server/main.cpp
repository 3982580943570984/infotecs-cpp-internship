#include "./server.h"

constexpr int PORT = 8080;
constexpr char IP_ADDRESS[] = "127.0.0.1";

int main() {
  Server server(IP_ADDRESS, PORT);
  
  std::cout << "Server is starting on port " << PORT << std::endl;
  server.start();

  return 0;
}