cmake_minimum_required(VERSION 3.5)

project(RandGraphAlgorithms LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(THREADS_PREFER_PTHREAD_FLAG ON)

add_executable(RandGraphAlgorithms main.cpp)
