cmake_minimum_required(VERSION 3.28)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra -pedantic -Werror -Wno-absolute-value)
if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g3")
endif()

project(Welle VERSION 0.1 LANGUAGES CXX)

add_library(${CMAKE_PROJECT_NAME} INTERFACE)

target_sources(${CMAKE_PROJECT_NAME} PUBLIC
    FILE_SET public_headers
    TYPE HEADERS
    BASE_DIRS include
    FILES include/Welle.hpp)

install(TARGETS ${CMAKE_PROJECT_NAME} FILE_SET public_headers)

if(CMAKE_BUILD_TYPE MATCHES Debug)
  add_subdirectory(tests)
  add_subdirectory(visualize)
endif()