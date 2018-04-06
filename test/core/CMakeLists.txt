include_directories(${basepath} ${CMAKE_SOURCE_DIR}/src)

set(core_srcs shmem_test.cc)
set(core_deps holodeck_core)

add_executable(core_tests ${core_srcs})
target_link_libraries(core_tests ${test_deps} ${core_deps} gtest gtest_main)

add_test(unit_tests run_core_tests)
