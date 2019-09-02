cmake_minimum_required(VERSION 2.8)
project(TEST_LISTEN)
set(TEST_LISTEN_VERSION_MAJOR 1)
set(TEST_LISTEN_VERSION_MINOR 0)
set(CMAKE_CXX_STANDARD 11)
include_directories(${CMAKE_SOURCE_DIR})
add_executable(server server.cpp)
add_executable(client client.cpp task.cpp)

option(USE_READ "accpet后调用read" ON)
if (USE_READ)
    add_definitions(-DUSE_READ)
endif()
set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(
    client
    pthread
)

install(
    TARGETS server client
    RUNTIME DESTINATION bin
)