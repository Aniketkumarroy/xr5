cmake_minimum_required(VERSION 3.11)
project(xr5 VERSION 1.0 LANGUAGES CXX)

include(${CMAKE_SOURCE_DIR}/common.cmake)

option(USE_BUNDLED_SPDLOG "Use bundled spdlog" ON)
set(LOG_DIR ${CMAKE_SOURCE_DIR}/logs/ CACHE STRING "Loggers directory")

if(USE_BUNDLED_SPDLOG)
    message(STATUS "Using bundled spdlog")
    add_subdirectory(3rdparty/spdlog EXCLUDE_FROM_ALL)
elseif()
    find_package(spdlog QUIET)
    if(spdlog_FOUND)
        message(STATUS "Using system-installed spdlog")
    else()
        message(STATUS "System spdlog not found, using FetchContent to get spdlog")
        include(FetchContent)
        FetchContent_Declare(
          spdlog
          GIT_REPOSITORY https://github.com/gabime/spdlog.git
          GIT_TAG v1.13.0
        )
        FetchContent_MakeAvailable(spdlog)
    endif()
endif()



add_subdirectory(utils)
add_subdirectory(memory)

set(LIBS xr5_utils xr5_memory)

add_executable(xr5 main.cpp)
target_link_libraries(xr5 PRIVATE ${LIBS})
