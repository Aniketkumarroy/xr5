cmake_minimum_required(VERSION 3.11)
project(xr5 VERSION 1.0 LANGUAGES CXX)

include(${CMAKE_SOURCE_DIR}/common.cmake)

add_subdirectory(utils)

set(LIBS xr5_utils)

add_executable(xr5 main.cpp)
target_link_libraries(xr5 PRIVATE ${LIBS})
