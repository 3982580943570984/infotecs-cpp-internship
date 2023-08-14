cmake_minimum_required(VERSION 3.12)
project(ClientServerExample)

set(CMAKE_CXX_STANDARD 14)

# Add the server executable
add_executable(server ./src/server.cpp)

# Add the client executable
add_executable(client ./src/client.cpp)