// test.cpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#include <gtest/gtest.h>
#include <mjxsdk/memory/impl/global_allocator.hpp>
#include <mjxsdk/memory/memory_resource.hpp>

namespace mjx {
    TEST(memory_resource, null_construct) {
        // test default and zero construction of memory_resource
        const memory_resource _Res0;
        const memory_resource _Res1(0);
        EXPECT_EQ(_Res0.data(), nullptr);
        EXPECT_EQ(_Res0.size(), 0);
        EXPECT_EQ(_Res1.data(), nullptr);
        EXPECT_EQ(_Res1.size(), 0);
        EXPECT_EQ(_Res0, _Res1);
    }

    TEST(memory_resource, copy_construct) {
        // test construction of memory_resource from another instance
        constexpr size_t _Size = 64;
        const memory_resource _Res0(_Size);
        const memory_resource _Res1 = _Res0;
        EXPECT_EQ(::memcmp(_Res0.data(), _Res1.data(), _Size), 0);
        EXPECT_EQ(_Res0.size(), _Res1.size());
    }

    TEST(memory_resource, move_construct) {
        // test construction of memory_resource from another instance
        constexpr size_t _Size = 128;
        memory_resource _Res0(_Size);
        const void* const _Data     = _Res0.data();
        const memory_resource _Res1 = ::std::move(_Res0);
        EXPECT_EQ(_Res0.data(), nullptr);
        EXPECT_EQ(_Res0.size(), 0);
        EXPECT_EQ(_Res1.data(), _Data);
        EXPECT_EQ(_Res1.size(), _Size);
    }

    TEST(memory_resource, copy_assign) {
        // test copy assignment of memory_resource
        constexpr size_t _Size = 256;
        const memory_resource _Res0(_Size);
        memory_resource _Res1;
        _Res1 = _Res0;
        EXPECT_EQ(::memcmp(_Res0.data(), _Res1.data(), _Size), 0);
        EXPECT_EQ(_Res0.size(), _Res1.size());
    }

    TEST(memory_resource, move_assign) {
        // test move assignment of memory_resource
        constexpr size_t _Size = 512;
        memory_resource _Res0(_Size);
        const void* const _Data = _Res0.data();
        memory_resource _Res1;
        _Res1 = ::std::move(_Res0);
        EXPECT_EQ(_Res0.data(), nullptr);
        EXPECT_EQ(_Res0.size(), 0);
        EXPECT_EQ(_Res1.data(), _Data);
        EXPECT_EQ(_Res1.size(), _Size);
    }

    TEST(memory_resource, empty) {
        // test checking whether the resource is empty
        memory_resource _Res;
        EXPECT_TRUE(_Res.empty());

        // after assigning a non-empty resource, the resource shouldn't be empty
        _Res = memory_resource(1024);
        EXPECT_FALSE(_Res.empty());
    }

    TEST(memory_resource, data) {
        // test access to the data of the resource
        memory_resource _Res;
        EXPECT_EQ(_Res.data(), nullptr);

        // after assigning a new non-empty resource, the resource shouldn't be empty
        _Res = memory_resource(2048);
        EXPECT_NE(_Res.data(), nullptr);
    }

    TEST(memory_resource, size) {
        // test access to the size of the resource
        constexpr size_t _Size = 4096;
        const memory_resource _Res(_Size);
        EXPECT_EQ(_Res.size(), _Size);
    }

    TEST(memory_resource, contains) {
        // test checking whether the resource contains a specific memory block
        constexpr size_t _Size = 8192;
        const memory_resource _Res(_Size);
        const unsigned char* const _Address = static_cast<const unsigned char*>(_Res.data());
        EXPECT_FALSE(_Res.contains(nullptr, 0));
        EXPECT_FALSE(_Res.contains(_Address, 0));
        EXPECT_FALSE(_Res.contains(nullptr, 2048));
        EXPECT_FALSE(_Res.contains(_Address - 1, 4096));
        EXPECT_FALSE(_Res.contains(_Address, _Size + 1));
        EXPECT_FALSE(_Res.contains(_Address + 1, _Size));
        EXPECT_TRUE(_Res.contains(_Address, _Size));
        EXPECT_TRUE(_Res.contains(_Address + (_Size / 2), _Size / 2));
    }

    TEST(memory_resource, swap) {
        // test swap functionality of memory_resource
        constexpr size_t _Size0 = 16384;
        constexpr size_t _Size1 = 32768;
        memory_resource _Res0(_Size0);
        memory_resource _Res1(_Size1);
        const void* const _Data0 = _Res0.data();
        const void* const _Data1 = _Res1.data();
        _Res0.swap(_Res1);
        EXPECT_EQ(_Res0.data(), _Data1);
        EXPECT_EQ(_Res0.size(), _Size1);
        EXPECT_EQ(_Res1.data(), _Data0);
        EXPECT_EQ(_Res1.size(), _Size0);
    }

    TEST(memory_resource, release) {
        // test release functionality of memory_resource
        constexpr size_t _Size = 65536;
        memory_resource _Res(_Size);
        void* const _Data = _Res.data();
        const auto& _Pair = _Res.release();
        EXPECT_EQ(_Pair.ptr, _Data);
        EXPECT_EQ(_Pair.size, _Size);

        // deallocate the released pointer to avoid a memory leak
        mjxsdk_impl::_Get_internal_allocator().deallocate(_Data, _Size);
    }

    TEST(memory_resource, destroy) {
        // test destroy functionality of memory_resource
        constexpr size_t _Size = 131072;
        memory_resource _Res(_Size);
        EXPECT_NE(_Res.data(), nullptr);
        EXPECT_EQ(_Res.size(), _Size);

        // after calling destroy(), the resource should be empty
        _Res.destroy();
        EXPECT_EQ(_Res.data(), nullptr);
        EXPECT_EQ(_Res.size(), 0);
    }
} // namespace mjx