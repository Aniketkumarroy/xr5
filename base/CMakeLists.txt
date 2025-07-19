cmake_minimum_required(VERSION 3.11)

get_filename_component(PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(xr5_${PROJECT_DIR} LANGUAGES CXX)

include(${CMAKE_SOURCE_DIR}/common.cmake)

set(XR5_BASE_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(XR5_BASE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB_RECURSE BASE_SRC CONFIGURE_DEPENDS
    ${XR5_BASE_SRC_DIR}/*.cc
)

# Create static library
add_library(${PROJECT_NAME} STATIC ${BASE_SRC})
target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${XR5_BASE_INCLUDE_DIR}> $<INSTALL_INTERFACE:include>)
target_link_libraries(${PROJECT_NAME} PUBLIC xr5_utils)

set_target_properties(${PROJECT_NAME} PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_PATH}
)
