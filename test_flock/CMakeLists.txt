cmake_minimum_required(VERSION 2.8)
project(TEST_FLOCK)
set(TEST_FLOCK_VERSION_MAJOR 1)
set(TEST_FLOCK_VERSION_MINOR 0)
set(CMAKE_CXX_STANDARD 14)

add_executable(test_flock main.cpp file_locker.cpp)
target_link_libraries(
    test_flock
    pthread
)
set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR})
install(
    TARGETS test_flock
    RUNTIME DESTINATION bin
)