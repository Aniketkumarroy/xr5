# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set compiler warnings
add_compile_options(-Wall -Wextra -Wpedantic)

# Determine OS, Arch, Bitness
string(TOLOWER "${CMAKE_SYSTEM_NAME}" OS_NAME)
string(TOLOWER "${CMAKE_SYSTEM_PROCESSOR}" ARCH_NAME)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(BITNESS 64)
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
    set(BITNESS 32)
else()
    set(BITNESS unknown)
endif()

# Output directory for libraries
set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/lib/${OS_NAME}${BITNESS}-${ARCH_NAME}")
