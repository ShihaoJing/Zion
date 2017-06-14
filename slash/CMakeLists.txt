
set(SOURCE_FILES main.cpp Server.cpp Connection.cpp)
add_executable(main ${SOURCE_FILES})

FIND_PACKAGE(Boost COMPONENTS thread system REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})

target_link_libraries(main ${Boost_LIBRARIES})