cmake_minimum_required(VERSION 3.10)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra -pedantic -Werror)

project(WelleTests VERSION 0.1 LANGUAGES CXX)

add_executable(WelleTests Tests.cpp)

find_package(Boost 1.85.0 REQUIRED COMPONENTS unit_test_framework)
message("Boost Path: " ${Boost_LIBRARY_DIRS} ", Libraries: " ${Boost_LIBRARIES})

target_include_directories(WelleTests PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(WelleTests Boost::unit_test_framework)
