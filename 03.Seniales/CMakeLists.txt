cmake_minimum_required(VERSION 2.8)
project(03.Seniales)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
add_executable(ejemplo1 main.cc SignalHandler.cpp)
