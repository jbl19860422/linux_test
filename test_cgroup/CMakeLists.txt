cmake_minimum_required(VERSION 2.8)
project(test)

file(GLOB_RECURSE SRCS ${CMAKE_SOURCE_DIR}/*.cpp ${CMAKE_SOURCE_DIR}/*.h)
add_executable(test ${SRCS})


