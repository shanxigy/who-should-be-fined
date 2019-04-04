set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall")
find_package(OTExtension REQUIRED)
find_package(Boost REQUIRED COMPONENTS system thread)

add_executable(test test.cpp)
target_link_libraries(test OTExtension::otextension)
include_directories(test "/usr/local/include")

add_executable(otmain otmain.cpp)
target_link_libraries(otmain OTExtension::otextension)
include_directories(otmain "/usr/local/include")
