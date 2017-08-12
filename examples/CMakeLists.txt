cmake_minimum_required(VERSION 3.2)
project(Zion_examples)

add_executable(example example.cpp)
target_link_libraries(example ${Boost_LIBRARIES})