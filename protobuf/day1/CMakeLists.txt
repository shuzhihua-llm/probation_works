cmake_minimum_required(VERSION 3.10)
project(protobuf_day1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Protobuf REQUIRED)

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS person.proto)

add_executable(protobuf_demo main.cpp ${PROTO_SRCS})
target_include_directories(protobuf_demo PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(protobuf_demo ${Protobuf_LIBRARIES})
