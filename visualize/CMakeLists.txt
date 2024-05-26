set(CMAKE_SHARED_MODULE_PREFIX "")

add_library(WelleVisualize MODULE
  PythonWrapper.cpp)

find_package(Python3 COMPONENTS Development)
find_package(Boost 1.85.0 REQUIRED COMPONENTS python)

target_include_directories(WelleVisualize PRIVATE ${Boost_INCLUDE_DIRS} ${Python3_INCLUDE_DIRS})
target_link_libraries(WelleVisualize ${Boost_LIBRARIES} ${Python3_LIBRARIES})