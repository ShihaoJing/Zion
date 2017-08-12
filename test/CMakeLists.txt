cmake_minimum_required(VERSION 3.2)
project(Zion_tests)

add_executable(unittest unittest.cpp)
target_link_libraries(unittest gtest_main)
target_link_libraries(unittest ${Boost_LIBRARIES})
add_test(NAME example_test COMMAND example)