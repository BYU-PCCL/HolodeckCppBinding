cmake_minimum_required(VERSION 3.5)
project(holodeck_cpp_binding)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")

add_subdirectory(lib/googletest)
add_subdirectory(src)
add_subdirectory(test)

#add_test(runUnitTests runUnitTests) #included in all tutorials but I don't know what it actually does.
