cmake_minimum_required(VERSION 3.12)
project(decimal_for_cpp)

set(CMAKE_CXX_STANDARD 11)

find_package(Boost 1.57.0 COMPONENTS system filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIRS} include)
add_executable(BoostTest tests/runner.cpp)
target_link_libraries(BoostTest ${Boost_LIBRARIES})

add_executable(decimal_for_cpp include/decimal.h tests/runner.cpp tests/decimalTest.ipp)