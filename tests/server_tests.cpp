#include <cassert>
#include <vector>

#include "../server/server.h"

template<typename T>
void assertEqual(const T &actual, const T &expected) {
  if (actual != expected) {
    std::cerr << "Test failed. Expected: " << expected
              << ", but got: " << actual << std::endl;
    assert(false);
  }
}

auto simulateServerDataProcessing = [](const std::string &buffer) {
  auto buffer_len = strlen(buffer.c_str());
  auto buffer_val = std::stoi(buffer);

  return (buffer_len > 2 && buffer_val % 32 == 0
              ? std::string("Data is correct")
              : std::string("Error: Data is incorrect (invalid length or mod)"));
};

void testServerDataProcessing() {
  std::vector<std::pair<std::string, std::string>> test_cases = {
      {"10", "Error: Data is incorrect (invalid length or mod)"},
      {"11", "Error: Data is incorrect (invalid length or mod)"},
      {"32", "Error: Data is incorrect (invalid length or mod)"},
      {"111", "Error: Data is incorrect (invalid length or mod)"},
      {"128", "Data is correct"},
      {"160", "Data is correct"},
      {"222", "Error: Data is incorrect (invalid length or mod)"},
      {"256", "Data is correct"},
      {"288", "Data is correct"},
      {"320", "Data is correct"},
      {"333", "Error: Data is incorrect (invalid length or mod)"},
      {"352", "Data is correct"},
      {"384", "Data is correct"},
      {"444", "Error: Data is incorrect (invalid length or mod)"},
      {"480", "Data is correct"},
      {"512", "Data is correct"},
      {"555", "Error: Data is incorrect (invalid length or mod)"},
      {"575", "Error: Data is incorrect (invalid length or mod)"},
  };

  for (const auto &test_case : test_cases) {
    std::string buffer = test_case.first;
    auto result = simulateServerDataProcessing(buffer);
    assertEqual(result, test_case.second);
  }
}

int main() {
  testServerDataProcessing();

  std::cout << "All tests passed!" << std::endl;

  return 0;
}