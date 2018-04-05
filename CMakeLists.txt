cmake_minimum_required(VERSION 3.10)
project(holodeck_cpp_binding)

set(CMAKE_CXX_STANDARD 14)
set(gtest_SOURCE_DIR /usr/local/googletest/googletest)
set(gmock_SOURCE_DIR /usr.local/googletest/googlemock)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
include_directories(${gmock_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

include_directories(src)

add_executable(holodeck_cpp_binding main.cpp src/core/shmem.cc src/core/shmem.h src/core/holodeck_client.cc src/core/holodeck_client.h)