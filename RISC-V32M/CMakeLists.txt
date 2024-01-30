add_library(RISC-V32M STATIC RV32M.cpp hex.cpp utilities.cpp rv32Encoding.cpp)
target_include_directories(RISC-V32M PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)