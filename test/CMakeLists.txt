set(basepath ${CMAKE_CURRENT_SOURCE_DIR})
set(test_deps gtest gmock)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
include_directories(${gmock_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

set(gtest_SOURCE_DIR ${CMAKE_SOURCE_DIR}/googletest/googletest)
set(gmock_SOURCE_DIR ${CMAKE_SOURCE_DIR}/googletest/googlemock)
add_subdirectory(core)

#add_executable(run_all_tests ${core_srcs})
#target_link_libraries(run_all_tests ${test_deps} ${core_deps})
