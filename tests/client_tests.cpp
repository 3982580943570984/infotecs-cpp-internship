#include <algorithm>
#include <cassert>
#include <vector>

#include "../client/client.h"

template <typename T> void assertEqual(const T &actual, const T &expected) {
  if (actual != expected) {
    std::cerr << "Test failed. Expected: " << expected
              << ", but got: " << actual << std::endl;
    assert(false);
  }
}

auto simulateInputValidation = [](const std::string &input) {
  auto max_input_length = 64;

  return ((input.length() <= max_input_length &&
           std::all_of(input.begin(), input.end(), ::isdigit))
              ? true
              : false);
};

void testInputValidation() {
  std::vector<std::pair<std::string, bool>> test_cases = {
      {"11 11 11 11 11 11 11 11 11 11 11 11 11 11 11 11 11 11 11 11 11 11 11",
       false},
      {"11 11 11 11 11 11 11 11 11 11 11 11 11 11 11 11 11 11 11 11 11 ",
       false},
      {"111111111111111111AAAA111111111111111111111111", false},
      {"1234567890123456789", true},
      {"1234567890123456789@", false},
  };

  for (const auto &test_case : test_cases) {
    std::string buffer = test_case.first;
    auto result = simulateInputValidation(buffer);
    assertEqual(result, test_case.second);
  }
}

auto replacement = "KB";
auto simulateInputProcessing = [](std::string &input) {
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
};

void testInputProcessing() {
  std::vector<std::pair<std::string, std::string>> test_cases = {
      {"1111", "1111"},
      {"1221", "KBKB11"},
      {"2222", "KBKBKBKB"},
      {"3123", "33KB1"},
      {"123456789", "9KB7KB5KB3KB1"}};

  for (const auto &test_case : test_cases) {
    std::string buffer = test_case.first;
    auto result = simulateInputProcessing(buffer);
    assertEqual(result, test_case.second);
  }
}

auto simulateDigitsSumCalculation = [](const std::string &input) {
  int sum = 0;

  for (const auto &c : input) {
    if (c >= '0' && c <= '9') {
      sum += c - '0';
    }
  }

  return sum;
};

void testDigitsSumCalculation() {
  std::vector<std::pair<std::string, int>> test_cases = {
      {"1111", 4}, {"1KBKB1", 2}, {"KBKBKBKB", 0}, {"31KB3", 7}, {"1KB3KB5KB7KB9", 25}};

  for (const auto &test_case : test_cases) {
    std::string buffer = test_case.first;
    auto result = simulateDigitsSumCalculation(buffer);
    assertEqual(result, test_case.second);
  }
}

int main() {
  testInputValidation();
  testInputProcessing();
  testDigitsSumCalculation();

  std::cout << "All tests passed!" << std::endl;

  return 0;
}