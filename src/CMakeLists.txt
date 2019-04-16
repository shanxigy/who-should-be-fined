cmake_minimum_required (VERSION 3.14)
project (WSBF)

set(CMAKE_CXX_STANDARD 11)

include_directories(".")
add_executable (file-handler-test file-handler-test.cpp file-handler.cpp)