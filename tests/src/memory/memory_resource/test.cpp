// test.cpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#include <gtest/gtest.h>
#include <mjxsdk/memory/memory_resource.hpp>

namespace mjx {
    TEST(memory_resource, null_construct) {
        const memory_resource _Res0;
        const memory_resource _Res1(0);
        const memory_resource _Res2(nullptr, 0);
        EXPECT_EQ(_Res0.data(), nullptr);
        EXPECT_EQ(_Res0.size(), 0);
        EXPECT_EQ(_Res1.data(), nullptr);
        EXPECT_EQ(_Res1.size(), 0);
        EXPECT_EQ(_Res2.data(), nullptr);
        EXPECT_EQ(_Res2.size(), 0);
        EXPECT_EQ(_Res0, _Res1);
        EXPECT_EQ(_Res0, _Res2);
        EXPECT_EQ(_Res1, _Res2);
    }

    TEST(memory_resource, copy_construct) {
        constexpr size_t _Size = 16;
        memory_resource _Res0(_Size);

        // fill the resource with 0xFF to distinguish it from the default-initialized buffer
        ::memset(_Res0.data(), 0xFF, _Size);

        memory_resource _Res1 = _Res0;
        EXPECT_EQ(::memcmp(_Res0.data(), _Res1.data(), _Size), 0);
        EXPECT_EQ(_Res0.size(), _Res1.size());
    }

    TEST(memory_resource, copy_construct_from_non_owned) {
        constexpr size_t _Size     = 32;
        unsigned char _Data[_Size] = {L'\0'};
        const memory_resource _Res0(_Data, _Size);
        EXPECT_FALSE(_Res0.owns());

        // copy of a non-owned resource is always owned
        const memory_resource _Res1 = _Res0;
        EXPECT_TRUE(_Res1.owns());
    }

    TEST(memory_resource, move_construct) {
        constexpr size_t _Size = 64;
        memory_resource _Res0(_Size);
        const void* const _Data     = _Res0.data();
        const memory_resource _Res1 = ::std::move(_Res0);
        EXPECT_EQ(_Res0.data(), nullptr);
        EXPECT_EQ(_Res0.size(), 0);
        EXPECT_EQ(_Res1.data(), _Data);
        EXPECT_EQ(_Res1.size(), _Size);
    }

    TEST(memory_resource, move_construct_from_non_owned) {
        constexpr size_t _Size     = 128;
        unsigned char _Data[_Size] = {L'\0'};
        memory_resource _Res0(_Data, _Size);
        EXPECT_FALSE(_Res0.owns());

        // move of a non-owned resource is never owned
        const memory_resource _Res1 = ::std::move(_Res0);
        EXPECT_FALSE(_Res1.owns());
    }

    TEST(memory_resource, external_construct) {
        constexpr size_t _Size     = 256;
        unsigned char _Data[_Size] = {'\0'};
        const memory_resource _Res(_Data, _Size);
        EXPECT_EQ(_Res.data(), _Data);
        EXPECT_EQ(_Res.size(), _Size);
        EXPECT_FALSE(_Res.owns());
    }

    TEST(memory_resource, copy_assign) {
        constexpr size_t _Size = 384;
        memory_resource _Res0(_Size);

        // fill the resource with 0xFF to distinguish it from the default-initialized buffer
        ::memset(_Res0.data(), 0xFF, _Size);

        memory_resource _Res1;
        _Res1 = _Res0;
        EXPECT_EQ(::memcmp(_Res0.data(), _Res1.data(), _Size), 0);
        EXPECT_EQ(_Res0.size(), _Res1.size());
    }

    TEST(memory_resource, copy_assign_from_non_owned) {
        constexpr size_t _Size     = 512;
        unsigned char _Data[_Size] = {'\0'};
        const memory_resource _Res0(_Data, _Size);
        EXPECT_FALSE(_Res0.owns());

        // copy of a non-owned resource is always owned
        memory_resource _Res1;
        _Res1 = _Res0;
        EXPECT_TRUE(_Res1.owns());
    }

    TEST(memory_resource, move_assign) {
        constexpr size_t _Size = 640;
        memory_resource _Res0(_Size);
        const void* const _Data = _Res0.data();
        memory_resource _Res1;
        _Res1 = ::std::move(_Res0);
        EXPECT_EQ(_Res0.data(), nullptr);
        EXPECT_EQ(_Res0.size(), 0);
        EXPECT_EQ(_Res1.data(), _Data);
        EXPECT_EQ(_Res1.size(), _Size);
    }

    TEST(memory_resource, move_assign_from_non_owned) {
        constexpr size_t _Size     = 768;
        unsigned char _Data[_Size] = {'\0'};
        memory_resource _Res0(_Data, _Size);
        EXPECT_FALSE(_Res0.owns());

        // move of a non-owned resource is never owned
        memory_resource _Res1;
        _Res1 = ::std::move(_Res0);
        EXPECT_FALSE(_Res1.owns());
    }

    TEST(memory_resource, empty) {
        memory_resource _Res;
        EXPECT_TRUE(_Res.empty());

        // assigning non-empty resource makes it non-empty
        _Res = memory_resource(896);
        EXPECT_FALSE(_Res.empty());

        // destroying a resource makes it empty
        _Res.destroy();
        EXPECT_TRUE(_Res.empty());
    }

    TEST(memory_resource, data) {
        memory_resource _Res;
        EXPECT_EQ(_Res.data(), nullptr);

        // assigning non-empty resource makes it non-empty
        _Res = memory_resource(1024);
        EXPECT_NE(_Res.data(), nullptr);
    }

    TEST(memory_resource, size) {
        constexpr size_t _Size = 1280;
        const memory_resource _Res(_Size);
        EXPECT_EQ(_Res.size(), _Size);
    }

    TEST(memory_resource, owns) {
        // empty resource is never owned
        memory_resource _Res0;
        EXPECT_FALSE(_Res0.owns());

        // internal buffer is always owned
        _Res0 = memory_resource(1536);
        EXPECT_TRUE(_Res0.owns());

        // external buffer is never owned
        constexpr size_t _Data_size     = 1792;
        unsigned char _Data[_Data_size] = {0};
        _Res0 = memory_resource(_Data, _Data_size);
        EXPECT_FALSE(_Res0.owns());

        // copy of external buffer becomes owned
        const memory_resource _Res1 = _Res0;
        EXPECT_TRUE(_Res1.owns());
    }

    TEST(memory_resource, contains) {
        constexpr size_t _Size = 2048;
        const memory_resource _Res(_Size);
        const unsigned char* const _Address = static_cast<const unsigned char*>(_Res.data());
        EXPECT_FALSE(_Res.contains(nullptr, 1024));
        EXPECT_FALSE(_Res.contains(_Address - 1, _Size));
        EXPECT_FALSE(_Res.contains(_Address, _Size + 1));
        EXPECT_FALSE(_Res.contains(_Address + 1, _Size));
        EXPECT_TRUE(_Res.contains(nullptr, 0));
        EXPECT_TRUE(_Res.contains(_Address, 0));
        EXPECT_TRUE(_Res.contains(_Address, _Size));
        EXPECT_TRUE(_Res.contains(_Address + (_Size / 2), _Size / 2));
    }

    TEST(memory_resource, swap) {
        constexpr size_t _Size0 = 2560;
        constexpr size_t _Size1 = 3072;
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

    TEST(memory_resource, swap_with_non_owned) {
        constexpr size_t _Size0     = 3584;
        constexpr size_t _Size1     = 3840;
        unsigned char _Data[_Size0] = {'\0'};
        memory_resource _Res0(_Data, _Size0);
        memory_resource _Res1(_Size1);
        EXPECT_FALSE(_Res0.owns());
        EXPECT_TRUE(_Res1.owns());

        _Res0.swap(_Res1);
        EXPECT_TRUE(_Res0.owns());
        EXPECT_FALSE(_Res1.owns());
    }

    TEST(memory_resource, destroy) {
        constexpr size_t _Size = 4096;
        memory_resource _Res(_Size);
        EXPECT_NE(_Res.data(), nullptr);
        EXPECT_EQ(_Res.size(), _Size);
        EXPECT_TRUE(_Res.owns());

        // destroying resource makes it empty
        _Res.destroy();
        EXPECT_EQ(_Res.data(), nullptr);
        EXPECT_EQ(_Res.size(), 0);
        EXPECT_FALSE(_Res.owns());
    }
} // namespace mjx