cmake_minimum_required(VERSION 3.10)
project(project-name)

set(CMAKE_CXX_STANDARD 11)

include_directories(src)

add_executable(main src/main.cpp)

enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(main_test tests/main_test.cpp)
target_link_libraries(main_test ${GTEST_LIBRARIES} pthread)

add_test(NAME main_test COMMAND main_test)