cmake_minimum_required(VERSION 3.14)
project(temp)

set(CMAKE_CXX_STANDARD 11)

add_executable(temp main.cpp main.cpp cpu.cpp cpu.h memory.cpp memory.h)