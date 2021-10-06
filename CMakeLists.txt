cmake_minimum_required(VERSION 3.0.0)
project(SingleLinkedList VERSION 0.1.0)

add_executable(SingleLinkedList main.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -std=c++17 -O3 -Wall -g ")