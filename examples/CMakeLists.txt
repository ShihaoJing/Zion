cmake_minimum_required(VERSION 3.7)
project(Zion_examples)

add_executable(example example.cpp)
target_link_libraries(example ${Boost_LIBRARIES})