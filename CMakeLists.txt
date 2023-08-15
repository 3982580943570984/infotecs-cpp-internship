cmake_minimum_required(VERSION 3.12)
project(ClientServerExample)

set(CMAKE_CXX_STANDARD 14)

# Server executable
add_executable(server ./server/main.cpp ./server/server.cpp ./other/utils.cpp)

# Client executable
add_executable(client ./client/main.cpp ./client/client.cpp ./other/utils.cpp)

# Server tests executable
add_executable(server_tests ./tests/server_tests.cpp)

# Client tests executable
add_executable(client_tests ./tests/client_tests.cpp)