cmake_minimum_required(VERSION 3.11)

get_filename_component(PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(xr5_${PROJECT_DIR} LANGUAGES CXX)

include(${CMAKE_SOURCE_DIR}/common.cmake)

option(TEST_EN "Build tests for utils library" OFF)

set(XR5_UTILS_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(XR5_UTILS_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(CONFIG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/config)

set(THIRD_PARTY_LIBS spdlog)

file(GLOB_RECURSE SRC CONFIGURE_DEPENDS
    ${XR5_UTILS_SRC_DIR}/*.cc
)

# Create static library
add_library(${PROJECT_NAME} STATIC ${SRC})
target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${XR5_UTILS_INCLUDE_DIR}> $<INSTALL_INTERFACE:include>)
target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CONFIG_DIR}> $<INSTALL_INTERFACE:config>)
target_link_libraries(${PROJECT_NAME} PUBLIC ${THIRD_PARTY_LIBS})
target_compile_definitions(${PROJECT_NAME} PUBLIC LOG_DIR="${LOG_DIR}")

# For organizing output (optional)
set_target_properties(${PROJECT_NAME} PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_PATH}
)

if(${FULL_BUILD})
  add_executable(${PROJECT_NAME}_demo demo/utils_demo.cc)
  target_include_directories(${PROJECT_NAME}_demo PUBLIC $<BUILD_INTERFACE:${XR5_UTILS_INCLUDE_DIR}> $<INSTALL_INTERFACE:include>)
  target_include_directories(${PROJECT_NAME}_demo PUBLIC $<BUILD_INTERFACE:${CONFIG_DIR}> $<INSTALL_INTERFACE:config>)
  target_link_libraries(${PROJECT_NAME}_demo PRIVATE ${THIRD_PARTY_LIBS} )
  add_custom_command(TARGET ${PROJECT_NAME}_demo POST_BUILD COMMAND ${CMAKE_STRIP} $<TARGET_FILE:${PROJECT_NAME}_demo> )
# install
install(TARGETS ${PROJECT_NAME}_demo
    RUNTIME DESTINATION bin
)
endif(${FULL_BUILD})

# install
install(TARGETS ${PROJECT_NAME}
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
)

install(DIRECTORY ${XR5_UTILS_INCLUDE_DIR}/
    DESTINATION include
)

