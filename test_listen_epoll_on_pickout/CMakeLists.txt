cmake_minimum_required(VERSION 2.8)
project(TEST_LISTEN)
set(TEST_LISTEN_VERSION_MAJOR 1)
set(TEST_LISTEN_VERSION_MINOR 0)
############ EPOLL OPTION ##############
option(USE_EPOLL_ET "USE ET FOR EPOLL?" ON)
if (USE_EPOLL_ET)
    add_definitions(-DUSE_EPOLL_ET)
endif()

add_executable(server server.cpp)
add_executable(client client.cpp)
set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR})
install(
    TARGETS server client
    RUNTIME DESTINATION bin
)