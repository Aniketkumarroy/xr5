cmake_minimum_required(VERSION 3.11)

get_filename_component(PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(xr5_${PROJECT_DIR} LANGUAGES CXX)

include(${CMAKE_SOURCE_DIR}/common.cmake)

option(TEST_EN "Build tests for utils library" OFF)

set(XR5_UTILS_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(XR5_UTILS_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB_RECURSE SRC CONFIGURE_DEPENDS
    ${XR5_UTILS_SRC_DIR}/*.cc
)

# Create static library
add_library(${PROJECT_NAME} STATIC ${SRC})
target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${XR5_UTILS_INCLUDE_DIR}> $<INSTALL_INTERFACE:include>)

# For organizing output (optional)
set_target_properties(${PROJECT_NAME} PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_PATH}
)

# Optional: add tests
if(TEST_EN)
    enable_testing()
    add_subdirectory(tests)
endif()
