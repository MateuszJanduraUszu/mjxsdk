# CMakeLists.txt

# Copyright (c) Mateusz Jandura. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.21.0)
project(mjxsdk_benchmarks LANGUAGES CXX)

# build Google Benchmark before building any benchmarks
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
add_subdirectory(google-benchmark "${CMAKE_CURRENT_BINARY_DIR}/google-benchmark" EXCLUDE_FROM_ALL)
if(WIN32 AND CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    # suppress -Winvalid-offsetof warning to compile Google Benchmark with Clang on Windows
    target_compile_options(benchmark PRIVATE -Wno-invalid-offsetof)
endif()

function(add_isolated_benchmark benchmark_name benchmark_path)
    # create an executable for the specified benchmark, linking against necessary libraries
    add_executable(${benchmark_name} ${benchmark_path})
    target_compile_definitions(${benchmark_name} PRIVATE BENCHMARK_STATIC_DEFINE)
    target_compile_features(${benchmark_name} PRIVATE cxx_std_20)
    target_include_directories(${benchmark_name} PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/../src"
        "${benchmark_SOURCE_DIR}/include"
    )
    target_link_libraries(${benchmark_name} PRIVATE benchmark mjxsdk)
    if(WIN32)
        # link against Shlwapi.lib, a Windows-specific statically-linked library
        target_link_libraries(${benchmark_name} PRIVATE Shlwapi)
    endif()
endfunction()

# Currently, there are no compatible benchmarks. However, the main build component
# has been implemented. Once sufficient progress is made, benchmarks will be added here.