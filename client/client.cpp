#include "./client.h"

Client::Client() {}

Client::Client(std::string ip_address, int port)
    : ip_address(ip_address), port(port) {}

Client::~Client() {}

bool Client::isValidInput(const std::string &input) {
  return input.length() <= max_input_length &&
         std::all_of(input.begin(), input.end(), ::isdigit);
}

std::string Client::processInput(std::string input) {
  std::sort(input.begin(), input.end(), std::greater<char>());

  std::string output;

  for (const auto &c : input) {
    if ((c - '0') % 2 == 0) {
      output += replacement;
    } else {
      output += c;
    }
  }

  return output;
}

void Client::userInputThread() {
  while (true) {
    std::string input;
    std::getline(std::cin, input);
    std::cout << std::endl;

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

int Client::calculateDigitsSum(const std::string &str) {
  int sum = 0;

  for (const auto &c : str) {
    if (c >= '0' && c <= '9') {
      sum += c - '0';
    }
  }

  return sum;
}

void Client::serverCommunicationThread() {
  // Creating socket
  int socket_fd = createTCPIPv4Socket();

  // Setting socket to reuse address
  auto server_address = createIPv4Address(ip_address.c_str(), port);

  // Connecting to the server
  if (connect(socket_fd, (sockaddr *)server_address.get(),
              sizeof(*server_address)) == -1) {
    perror("Error: Failed to connect to the server");
    std::exit(EXIT_FAILURE);
  }

  while (true) {
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [this]() { return input_ready; });

    std::string temp = std::move(data);
    input_ready = false;
    std::cout << "Server communication thread has input: " << temp << std::endl;

    int sum = calculateDigitsSum(temp);
    std::cout << "Sum of all elements: " << sum << std::endl;

    std::string sum_str = std::to_string(sum);
    std::cout << "Sending sum to the server" << std::endl;
    if (send(socket_fd, sum_str.c_str(), sum_str.length(), 0) == -1) {
      std::cerr << "Error: Failed to send data to the server" << std::endl;
      std::exit(EXIT_FAILURE);
    }
    std::cout << "Sent sum to the server" << std::endl;

    std::cout << std::endl;
  }
}

void Client::start() {
  std::thread t1(&Client::userInputThread, this);
  std::thread t2(&Client::serverCommunicationThread, this);

  t1.join();
  t2.join();
}