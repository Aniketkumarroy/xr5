cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(riscv32 VERSION 1.0)

add_subdirectory(RISC-V32M)

add_executable(riscv32 main.cpp)
target_link_libraries(riscv32 PUBLIC RISC-V32M)
