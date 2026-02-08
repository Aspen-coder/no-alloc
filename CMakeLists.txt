cmake_minimum_required(VERSION 3.20)
project(NoAllocSTL VERSION 0.1.0 LANGUAGES CXX)


# high C++ standards
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) # MISRA forbids compiler-specific extensions

# critical infrastructure compile flags
add_compile_options(
    -Wall -Wextra -Wpedantic -Wshadow -Wconversion
    -fno-exceptions 
    -fno-rtti
)

# auto static analysis
find_program(CLANG_TIDY_EXE NAMES "clang-tidy")
if(CLANG_TIDY_EXE)
    set(CMAKE_CXX_CLANG_TIDY
        ${CLANG_TIDY_EXE};
        -checks=-*,bugprone-*,performance-*,portability-*,modernize-*,cppcoreguidelines-*;
        -warnings-as-errors=*
    )
    message(STATUS "Clang-Tidy safety checks enabled")
endif()

# header-only library target
add_library(no_alloc INTERFACE)
target_include_directories(no_alloc INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Testing & Benchmarking
option(NO_ALLOC_BUILD_TESTS "Build unit tests" ON)
if(NO_ALLOC_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

option(NO_ALLOC_BUILD_BENCHMARKS "Build performance benchmarks" OFF)
if(NO_ALLOC_BUILD_BENCHMARKS)
    add_subdirectory(benchmarks)
endif()
