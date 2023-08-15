#include <algorithm>          /* for std::all_of, std::sort */
#include <arpa/inet.h>        /* for inet_ntop */
#include <condition_variable> /* for std::condition_variable */
#include <iostream>           /* for std::cout */
#include <mutex>              /* for std::mutex */
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>

constexpr int PORT = 8080;
constexpr char IP_ADDRESS[] = "127.0.0.1";
constexpr size_t MAX_INPUT_LENGTH = 64;
constexpr char REPLACEMENT[] = "KB";

std::mutex mtx;
std::condition_variable cv;
std::string data;
bool input_ready = false;

int createTCPIpV4Socket(void);

std::unique_ptr<sockaddr_in> createIPv4Address(const char* ip_address, uint16_t port);

bool isValidInput(const std::string &input) {
  return input.length() <= MAX_INPUT_LENGTH &&
         std::all_of(input.begin(), input.end(), ::isdigit);
}

std::string processInput(std::string input) {
  std::sort(input.begin(), input.end(), std::greater<char>());

  std::string output;

  for (const auto &c : input) {
    if ((c - '0') % 2 == 0) {
      output += REPLACEMENT;
    } else {
      output += c;
    }
  }

  return output;
}

void userInputThread() {
  while (true) {
    std::string input;
    std::getline(std::cin, input);

    if (!isValidInput(input)) {
      std::cout << "Error: Invalid input" << std::endl;
      continue;
    }

    std::unique_lock<std::mutex> lock(mtx);
    data = processInput(std::move(input));
    input_ready = true;
    cv.notify_one();
  }
}

int calculateDigitsSum(const std::string &str) {
  int sum = 0;
  
  for (const auto& c : str) {
    if (c >= '0' && c <= '9') {
      sum += c - '0';
    }
  }

  return sum;
}

void serverCommunicationThread() {
  // Creating socket
  int socket_fd = createTCPIpV4Socket();
  if (socket_fd == -1) {
    perror("Socket creation failed");
    exit(EXIT_FAILURE);
  }

  // Setting socket to reuse address
  auto serv_addr = createIPv4Address(IP_ADDRESS, 80);

  // Connecting to the server
  if (connect(socket_fd, (sockaddr *)serv_addr.get(), sizeof(*serv_addr)) == -1) {
    perror("Connection failed");
    exit(EXIT_FAILURE);
  }

  while (true) {
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, []() { return input_ready; });

    std::string temp = std::move(data);
    input_ready = false;
    std::cout << "Thread 2 has input: " << temp << std::endl;

    int sum = calculateDigitsSum(temp);
    std::cout << "Sum of all elements: " << sum << std::endl;

    std::string sum_str = std::to_string(sum);
    // check for error
    send(socket_fd, sum_str.c_str(), sum_str.length(), 0);
  }
}

int main() {
  std::thread t1(userInputThread);
  std::thread t2(serverCommunicationThread);

  t1.join();
  t2.join();

  return 0;
}

int createTCPIpV4Socket(void) { return socket(AF_INET, SOCK_STREAM, 0); }

std::unique_ptr<sockaddr_in> createIPv4Address(const char* ip_address, uint16_t port) {
  std::unique_ptr<sockaddr_in> address = std::make_unique<sockaddr_in>();
  address->sin_family = AF_INET;
  address->sin_port = htons(port);
  inet_pton(AF_INET, ip_address, &(address->sin_addr));
  return address;
}