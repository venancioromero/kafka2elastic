cmake_minimum_required(VERSION 3.12)
project(kafka2elastic)

set(CMAKE_CXX_STANDARD 14)

add_executable(kafka2elastic main.cpp)

target_link_libraries(kafka2elastic "/usr/local/lib/libconfig++.so")
target_link_libraries(kafka2elastic "/usr/local/lib/librdkafka++.so")


