cmake_minimum_required(VERSION 3.10)
project(flatbuffer_demo)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(flatbuffer_demo main.cpp)
target_include_directories(flatbuffer_demo PRIVATE /usr/include)
target_link_libraries(flatbuffer_demo flatbuffers)
