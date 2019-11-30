cmake_minimum_required(VERSION 3.15)
project(myserv)

set(CMAKE_CXX_STANDARD 17)

add_executable(mycl client.cpp)
add_executable(myserv server.cpp)
