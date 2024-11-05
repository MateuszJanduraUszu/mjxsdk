// test.cpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#include <gtest/gtest.h>
#include <mjxsdk/smart_pointer.hpp>

namespace mjx {
    TEST(unique_array, null_construct) {
        // test default and null construction of unique_array
        unique_array<int> _Unique0;
        unique_array<int> _Unique1(nullptr, 0);
        EXPECT_EQ(_Unique0.get(), nullptr);
        EXPECT_EQ(_Unique0.size(), 0);
        EXPECT_EQ(_Unique1.get(), nullptr);
        EXPECT_EQ(_Unique1.size(), 0);
    }

    TEST(unique_array, move_construct) {
        // test construction of unique_array from another instance
        unique_array<int> _Unique0 = ::mjx::make_unique_array<int>(8);
        const int* const _Ptr      = _Unique0.get();
        const size_t _Size         = _Unique0.size();
        unique_array<int> _Unique1 = ::std::move(_Unique0);
        EXPECT_EQ(_Unique0.get(), nullptr);
        EXPECT_EQ(_Unique0.size(), 0);
        EXPECT_EQ(_Unique1.get(), _Ptr);
        EXPECT_EQ(_Unique1.size(), _Size);
    }

    TEST(unique_array, pointer_construct) {
        // test construction of unique_array from a raw pointer and size
        constexpr size_t _Size = 16;
        int* const _Ptr        = ::mjx::create_object_array<int>(_Size);
        unique_array<int> _Unique(_Ptr, _Size); // destroys _Ptr
        EXPECT_EQ(_Unique.get(), _Ptr);
        EXPECT_EQ(_Unique.size(), _Size);
    }

    TEST(unique_array, null_assign) {
        // test assignment of null to unique_array
        unique_array<int> _Unique = ::mjx::make_unique_array<int>(32);
        _Unique                   = nullptr;
        EXPECT_EQ(_Unique.get(), nullptr);
        EXPECT_EQ(_Unique.size(), 0);
    }

    TEST(unique_array, move_assign) {
        // test move assignment of unique_array
        unique_array<int> _Unique0 = ::mjx::make_unique_array<int>(64);
        const int* const _Ptr      = _Unique0.get();
        const size_t _Size         = _Unique0.size();
        unique_array<int> _Unique1;
        _Unique1 = ::std::move(_Unique0);
        EXPECT_EQ(_Unique0.get(), nullptr);
        EXPECT_EQ(_Unique0.size(), 0);
        EXPECT_EQ(_Unique1.get(), _Ptr);
        EXPECT_EQ(_Unique1.size(), _Size);
    }

    TEST(unique_array, get) {
        // test access to the managed array of unique_array
        constexpr size_t _Size = 128;
        int* const _Ptr        = ::mjx::create_object_array<int>(_Size);
        unique_array<int> _Unique(_Ptr, _Size); // destroys _Ptr
        EXPECT_EQ(_Unique.get(), _Ptr);
    }

    TEST(unique_array, size) {
        // test access to the size of the managed array of unique_array
        constexpr size_t _Size = 256;
        int* const _Ptr        = ::mjx::create_object_array<int>(_Size);
        unique_array<int> _Unique(_Ptr, _Size); // destroys _Ptr
        EXPECT_EQ(_Unique.size(), _Size);
    }

    TEST(unique_array, release) {
        // test release of unique_array, transferring ownership
        constexpr size_t _Size = 512;
        int* const _Ptr        = ::mjx::create_object_array<int>(_Size);
        unique_array<int> _Unique(_Ptr, _Size);
        const auto _Pair = _Unique.release();
        EXPECT_EQ(_Unique.get(), nullptr);
        EXPECT_EQ(_Unique.size(), 0);
        EXPECT_EQ(_Pair.ptr, _Ptr);
        EXPECT_EQ(_Pair.size, _Size);
        ::mjx::delete_object_array(_Ptr, _Size);
    }

    TEST(unique_array, reset) {
        // test reset of unique_array, deleting the managed array
        constexpr size_t _Size    = 1024;
        unique_array<int> _Unique = ::mjx::make_unique_array<int>(_Size);
        EXPECT_NE(_Unique.get(), nullptr);
        EXPECT_EQ(_Unique.size(), _Size);
        _Unique.reset();
        EXPECT_EQ(_Unique.get(), nullptr);
        EXPECT_EQ(_Unique.size(), 0);
    }

    TEST(unique_array, swap) {
        // test swap functionality of unique_array
        constexpr size_t _Size0 = 2048;
        constexpr size_t _Size1 = 4096;
        int* const _Ptr0        = ::mjx::create_object_array<int>(_Size0);
        int* const _Ptr1        = ::mjx::create_object_array<int>(_Size1);
        unique_array<int> _Unique0(_Ptr0, _Size0); // destroys _Ptr0
        unique_array<int> _Unique1(_Ptr1, _Size1); // destroys _Ptr1
        _Unique0.swap(_Unique1);
        EXPECT_EQ(_Unique0.get(), _Ptr1);
        EXPECT_EQ(_Unique0.size(), _Size1);
        EXPECT_EQ(_Unique1.get(), _Ptr0);
        EXPECT_EQ(_Unique1.size(), _Size0);
    }

    TEST(unique_array, bounds_checking) {
        // test bounds checking for unique_array
        unique_array<int> _Unique = ::mjx::make_unique_array<int>(8192);
        bool _Caught              = false;
        try {
            (void) _Unique[_Unique.size()]; // intentionally raise resource_overrun exception
        } catch (const resource_overrun&) {
            _Caught = true;
        }

        EXPECT_TRUE(_Caught);
    }
} // namespace mjx