# CMakeLists.txt

# Copyright (c) Mateusz Jandura. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.21.0)
project(mjxsdk_library LANGUAGES CXX)

# check if the platform architecture is supported (MJX_PLATFORM_ARCH defined in presets)
if(NOT ${MJX_PLATFORM_ARCH} MATCHES "^x64$|^x86$")
    message(FATAL_ERROR "The MJXSDK library can only be built for x64 and x86 architectures.")
endif()

option(MJXSDK_BUILD_BENCHMARKS "Build MJXSDK library benchmarks" OFF)
option(MJXSDK_BUILD_TESTS "Build MJXSDK library tests" OFF)
option(MJXSDK_INSTALL_LIBRARY "Install MJXSDK library" OFF)

# build the MJXSDK library before building any benchmarks and tests
add_subdirectory(src "${CMAKE_CURRENT_BINARY_DIR}/library")

if(MJXSDK_BUILD_BENCHMARKS)
    add_subdirectory(benchmarks "${CMAKE_CURRENT_BINARY_DIR}/benchmarks")
endif()

if(MJXSDK_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests "${CMAKE_CURRENT_BINARY_DIR}/tests")
endif()