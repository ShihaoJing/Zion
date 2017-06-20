
set(SOURCE_FILES main.cpp server.cpp connection.cpp connection_manager.cpp request_parser.cpp)
add_executable(main ${SOURCE_FILES})

FIND_PACKAGE(Boost COMPONENTS thread system REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})

target_link_libraries(main ${Boost_LIBRARIES})