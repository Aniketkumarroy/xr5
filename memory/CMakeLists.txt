cmake_minimum_required(VERSION 3.11)

get_filename_component(PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(xr5_${PROJECT_DIR} LANGUAGES CXX)

include(${CMAKE_SOURCE_DIR}/common.cmake)

set(XR5_MEMORY_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(XR5_MEMORY_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB_RECURSE SRC CONFIGURE_DEPENDS
    ${XR5_MEMORY_SRC_DIR}/*.cc
)

set(LIBS xr5_utils)

# Create static library
add_library(${PROJECT_NAME} STATIC ${SRC})
target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${XR5_MEMORY_INCLUDE_DIR}> $<INSTALL_INTERFACE:include>)
target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBS})

# For organizing output (optional)
set_target_properties(${PROJECT_NAME} PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_PATH}
)

if(${FULL_BUILD})
  add_executable(${PROJECT_NAME}_demo demo/memory_demo.cc)
  target_include_directories(${PROJECT_NAME}_demo PUBLIC $<BUILD_INTERFACE:${XR5_MEMORY_INCLUDE_DIR}> $<INSTALL_INTERFACE:include>)
  target_link_libraries(${PROJECT_NAME}_demo PRIVATE ${LIBS})
  add_custom_command(TARGET ${PROJECT_NAME}_demo POST_BUILD COMMAND ${CMAKE_STRIP} $<TARGET_FILE:${PROJECT_NAME}_demo> )
endif(${FULL_BUILD})
