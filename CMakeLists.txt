cmake_minimum_required(VERSION 3.7)
project(Zion)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(slash bin)
set(prj_bin ${CMAKE_CURRENT_BINARY_DIR}/bin)