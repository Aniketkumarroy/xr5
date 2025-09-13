cmake_minimum_required(VERSION 3.11)

get_filename_component(PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(xr5_${PROJECT_DIR} LANGUAGES CXX)

include(${CMAKE_SOURCE_DIR}/common.cmake)


set(XR5_SIM_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(XR5_SIM_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB_RECURSE SRC CONFIGURE_DEPENDS
    ${XR5_SIM_SRC_DIR}/*.cc
)

set(LIBS xr5_utils)

# Create static library
add_library(${PROJECT_NAME} STATIC ${SRC})
target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${XR5_SIM_INCLUDE_DIR}> $<INSTALL_INTERFACE:include>)
target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBS})

# For organizing output (optional)
set_target_properties(${PROJECT_NAME} PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_PATH}
)

# install
install(TARGETS ${PROJECT_NAME}
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
)

install(DIRECTORY ${XR5_SIM_INCLUDE_DIR}/
    DESTINATION include
)