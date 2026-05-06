cmake_minimum_required(VERSION 3.22)
project(ngine
    VERSION 1.0.0
    DESCRIPTION "C++ Geometry Engine with Constraint Solving"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

include(CompilerWarnings)
include(Sanitizers)
include(Dependencies)

option(NGINE_BUILD_TESTS "Build unit tests" ON)
option(NGINE_BUILD_BENCHMARKS "Build benchmarks" OFF)
option(NGINE_ENABLE_SANITIZERS "Enable sanitizers" OFF)
option(NGINE_ENABLE_CLANG_TIDY "Enable clang-tidy" OFF)

if(NGINE_ENABLE_CLANG_TIDY)
    find_program(CLANG_TIDY_EXE NAMES clang-tidy)
    if(CLANG_TIDY_EXE)
        set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE}")
    endif()
endif()

add_subdirectory(src/logging)
add_subdirectory(src/core)
add_subdirectory(src/intersection)
add_subdirectory(src/constraints)
add_subdirectory(src/commands)
add_subdirectory(src/serialization)
add_subdirectory(src/spatial)
add_subdirectory(src/interface)
add_subdirectory(apps)

if(NGINE_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

if(NGINE_BUILD_BENCHMARKS)
    add_subdirectory(benchmarks)
endif()
