# CMakeLists.txt

# Copyright (c) Mateusz Jandura. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.21.0)
project(mjxsdk_tests LANGUAGES CXX)

# build Google Test as a static library before building any tests
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
add_subdirectory(google-test "${CMAKE_CURRENT_BINARY_DIR}/google-test" EXCLUDE_FROM_ALL)

function(add_isolated_test test_name test_path)
    # create an executable for the specified test, linking against necessary libraries
    add_executable(${test_name} ${test_path})
    target_compile_features(${test_name} PRIVATE cxx_std_20)
    target_include_directories(${test_name} PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/../src"
        "${gtest_SOURCE_DIR}/include"
    )
    target_link_libraries(${test_name} PRIVATE gtest gtest_main mjxsdk)
    add_test(NAME ${test_name} COMMAND ${test_name})
endfunction()

add_isolated_test(test_core_architecture_validation "src/core/architecture_validation/test.cpp")
add_isolated_test(test_core_version_encoding "src/core/version_encoding/test.cpp")
add_isolated_test(test_memory_allocators_compatibility "src/memory/allocators_compatibility/test.cpp")
add_isolated_test(test_memory_debug_memory_block "src/memory/debug_memory_block/test.cpp")
add_isolated_test(test_memory_endian "src/memory/endian/test.cpp")
add_isolated_test(test_memory_global_allocator "src/memory/global_allocator/test.cpp")
add_isolated_test(test_memory_memory_resource "src/memory/memory_resource/test.cpp")
add_isolated_test(test_memory_native_allocator "src/memory/native_allocator/test.cpp")
add_isolated_test(test_memory_object_allocator "src/memory/object_allocator/test.cpp")
add_isolated_test(test_memory_object_management "src/memory/object_management/test.cpp")
add_isolated_test(test_memory_shared_array "src/memory/shared_array/test.cpp")
add_isolated_test(test_memory_shared_ptr "src/memory/shared_ptr/test.cpp")
add_isolated_test(test_memory_unique_array "src/memory/unique_array/test.cpp")
add_isolated_test(test_memory_unique_ptr "src/memory/unique_ptr/test.cpp")

# use a custom target to combine all targets into a single one,
# this allows only one post-build call instead of per-test copying
add_custom_target(mjxsdk_and_tests ALL DEPENDS
    mjxsdk
    test_core_architecture_validation
    test_core_version_encoding
    test_memory_allocators_compatibility
    test_memory_debug_memory_block
    test_memory_endian
    test_memory_global_allocator
    test_memory_memory_resource
    test_memory_native_allocator
    test_memory_object_allocator
    test_memory_object_management
    test_memory_shared_array
    test_memory_shared_ptr
    test_memory_unique_array
    test_memory_unique_ptr
)
add_custom_command(TARGET mjxsdk_and_tests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "$<TARGET_FILE:mjxsdk>"
    "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>" # copy to the tests' output directory
)