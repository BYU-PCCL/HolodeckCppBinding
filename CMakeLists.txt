cmake_minimum_required(VERSION 3.10)
project(holodeck_cpp_binding)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(holodeck_cpp_binding main.cpp core/shmem.cc core/shmem.h core/holodeck_client.cc core/holodeck_client.h)