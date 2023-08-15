#include "./client.h"

constexpr int PORT = 8080;
constexpr char IP_ADDRESS[] = "127.0.0.1";

int main() {
  Client client(IP_ADDRESS, PORT);
  client.start();

  return 0;
}