#ifndef CLIENT_H
#define CLIENT_H

#include <algorithm>
#include <condition_variable>
#include <cstdio>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>

#include <arpa/inet.h>

#include "../other/utils.h"

class Client {
public:
  Client();
  Client(std::string ip_address, int port);
  ~Client();

  void start();

private:
  size_t max_input_length = 64;
  std::string replacement = "KB";

  std::string ip_address;
  int port;

  std::mutex mtx;
  std::condition_variable cv;
  std::string data;
  bool input_ready = false;

  bool isValidInput(const std::string &input);
  std::string processInput(std::string input);
  void userInputThread();
  int calculateDigitsSum(const std::string &str);
  void serverCommunicationThread();
};

#endif /* CLIENT_H */