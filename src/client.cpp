#include <algorithm>
#include <arpa/inet.h> /* inet_ntop */
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>

#define PORT 8080

std::mutex mtx;
std::condition_variable cv;
std::string data;
bool inputReady = false;

void thread1() {
  while (true) {
    std::string input;
    std::getline(std::cin, input);

    if (input.length() > 64 ||
        !std::all_of(input.begin(), input.end(), ::isdigit)) {
      std::cout << "Error: Invalid input" << std::endl;
      continue;
    }

    std::sort(input.begin(), input.end(), std::greater<char>());

    // std::transform(input.begin(), input.end(), input.begin(), [](char c) {
    //   if ((c - '0') % 2 == 0) {
    //     return 'K';
    //   }
    //   return c;
    // });

    // std::replace_if(
    //     input.begin(), input.end(), [](char c) { return (c - '0') % 2 == 0;
    //     }, 'K');

    std::string replacement = "KB";
    std::string output;

    for (const auto& c : input) {
      if ((c - '0') % 2 == 0) {
        output += replacement;
      } else {
        output += c;
      }
    }

    input = std::move(output);

    std::unique_lock<std::mutex> lock(mtx);
    data = input;
    inputReady = true;
    lock.unlock();

    cv.notify_one();
  }
}

void thread2() {
  struct sockaddr_in address;
  int sock = 0;
  struct sockaddr_in serv_addr;
  char buffer[1024] = {0};

  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    std::cout << "Socket creation error" << std::endl;
    return;
  }

  memset(&serv_addr, '0', sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);

  if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
    std::cout << "Invalid address/ Address not supported" << std::endl;
    return;
  }

  if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    std::cout << "Connection Failed" << std::endl;
    return;
  }

  while (true) {
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, []() { return inputReady; });

    std::string temp = data;
    data.clear();
    inputReady = false;
    lock.unlock();

    std::cout << "Thread 2 has input: " << temp << std::endl;

    int sum = 0;
    for (char c : temp) {
      if (c >= '0' && c <= '9') {
        sum += c - '0';
      }
    }

    std::cout << "Sum: " << sum << std::endl;
    std::string sum_str = std::to_string(sum);

    send(sock, sum_str.c_str(), sum_str.size(), 0);
  }
}

int main() {
  std::thread t1(thread1);
  std::thread t2(thread2);

  t1.join();
  t2.join();

  return 0;
}