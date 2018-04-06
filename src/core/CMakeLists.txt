include_directories(${basepath})

set(core_SRCS holodeck_client.cc
        holodeck_client.h
        shmem.cc
        shmem.h)

add_library(holodeck_core ${core_SRCS})

find_package(Threads)
target_link_libraries(holodeck_core ${CMAKE_THREAD_LIBS_INIT} rt)