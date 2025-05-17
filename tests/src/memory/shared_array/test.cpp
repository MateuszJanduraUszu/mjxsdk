// test.cpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#include <gtest/gtest.h>
#include <mjxsdk/memory/smart_pointer.hpp>

namespace mjx {
    TEST(shared_array, null_construct) {
        // test default and null construction of shared_array
        const shared_array<int> _Shared0;
        const shared_array<int> _Shared1(nullptr, 0);
        EXPECT_EQ(_Shared0.get(), nullptr);
        EXPECT_EQ(_Shared0.size(), 0);
        EXPECT_EQ(_Shared0.use_count(), 0);
        EXPECT_EQ(_Shared1.get(), nullptr);
        EXPECT_EQ(_Shared1.size(), 0);
        EXPECT_EQ(_Shared1.use_count(), 0);
    }

    TEST(shared_array, copy_construct) {
        // test construction of shared_array from another instance
        const shared_array<int> _Shared0 = ::mjx::make_shared_array<int>(4);
        const shared_array<int> _Shared1 = _Shared0;
        EXPECT_EQ(_Shared0.get(), _Shared1.get());
        EXPECT_EQ(_Shared0.size(), _Shared1.size());
        EXPECT_EQ(_Shared0.use_count(), 2);
        EXPECT_EQ(_Shared1.use_count(), 2);
    }

    TEST(shared_array, move_construct) {
        // test construction of shared_array from another instance
        shared_array<int> _Shared0       = ::mjx::make_shared_array<int>(8);
        const shared_array<int> _Shared1 = ::std::move(_Shared0);
        EXPECT_EQ(_Shared0.get(), nullptr);
        EXPECT_EQ(_Shared0.size(), 0);
        EXPECT_EQ(_Shared0.use_count(), 0);
        EXPECT_NE(_Shared1.get(), nullptr);
        EXPECT_EQ(_Shared1.size(), 8);
        EXPECT_EQ(_Shared1.use_count(), 1);
    }

    TEST(shared_array, pointer_construct) {
        // test construction of shared_array from a raw pointer
        constexpr size_t _Size = 16;
        int* const _Ptr        = ::mjx::create_object_array<int>(_Size);
        const shared_array<int> _Shared(_Ptr, _Size); // destroys _Ptr
        EXPECT_EQ(_Shared.get(), _Ptr);
        EXPECT_EQ(_Shared.size(), _Size);
        EXPECT_EQ(_Shared.use_count(), 1);
    }

    TEST(shared_array, unique_construct) {
        // test construction of shared_array from unique_array
        constexpr size_t _Size = 32;
        int* const _Ptr        = ::mjx::create_object_array<int>(_Size);
        unique_array<int> _Unique(_Ptr, _Size);
        const shared_array<int> _Shared(::std::move(_Unique)); // destroys _Ptr
        EXPECT_EQ(_Unique.get(), nullptr);
        EXPECT_EQ(_Unique.size(), 0);
        EXPECT_EQ(_Shared.get(), _Ptr);
        EXPECT_EQ(_Shared.size(), _Size);
        EXPECT_EQ(_Shared.use_count(), 1);
    }

    TEST(shared_array, copy_assign) {
        // test copy assignment of shared_array
        const shared_array<int> _Shared0 = ::mjx::make_shared_array<int>(64);
        shared_array<int> _Shared1;
        _Shared1 = _Shared0;
        EXPECT_EQ(_Shared0.get(), _Shared1.get());
        EXPECT_EQ(_Shared0.size(), _Shared1.size());
        EXPECT_EQ(_Shared0.use_count(), 2);
        EXPECT_EQ(_Shared1.use_count(), 2);
    }

    TEST(shared_array, move_assign) {
        // test move assignment of shared_array
        shared_array<int> _Shared0 = ::mjx::make_shared_array<int>(128);
        shared_array<int> _Shared1;
        _Shared1 = ::std::move(_Shared0);
        EXPECT_EQ(_Shared0.get(), nullptr);
        EXPECT_EQ(_Shared0.size(), 0);
        EXPECT_EQ(_Shared0.use_count(), 0);
        EXPECT_NE(_Shared1.get(), nullptr);
        EXPECT_EQ(_Shared1.size(), 128);
        EXPECT_EQ(_Shared1.use_count(), 1);
    }

    TEST(shared_array, unique_assign) {
        // test assignment of unique_array to shared_array
        constexpr size_t _Size = 256;
        int* const _Ptr        = ::mjx::create_object_array<int>(_Size);
        unique_array<int> _Unique(_Ptr, _Size);
        shared_array<int> _Shared;
        _Shared = ::std::move(_Unique); // destroys _Ptr
        EXPECT_EQ(_Unique.get(), nullptr);
        EXPECT_EQ(_Unique.size(), 0);
        EXPECT_EQ(_Shared.get(), _Ptr);
        EXPECT_EQ(_Shared.size(), _Size);
        EXPECT_EQ(_Shared.use_count(), 1);
    }

    TEST(shared_array, get) {
        // test access to the managed array of shared_array
        constexpr size_t _Size = 512;
        int* const _Ptr        = ::mjx::create_object_array<int>(_Size);
        const shared_array<int> _Shared(_Ptr, _Size); // destroys _Ptr
        EXPECT_EQ(_Shared.get(), _Ptr);
    }

    TEST(shared_array, size) {
        // test access to the size of the managed array of shared_array
        constexpr size_t _Size          = 1024;
        const shared_array<int> _Shared = ::mjx::make_shared_array<int>(_Size);
        EXPECT_EQ(_Shared.size(), _Size);
    }

    TEST(shared_array, use_count) {
        // test managed array access counting of shared_array
        const shared_array<int> _Shared0 = ::mjx::make_shared_array<int>(2048);
        EXPECT_EQ(_Shared0.use_count(), 1);
        {
            const shared_array<int> _Shared1 = _Shared0;
            const shared_array<int> _Shared2 = _Shared0;
            EXPECT_EQ(_Shared0.use_count(), 3);
        }

        // once _Shared1 and _Shared2 are destroyed, _Shared0 is unique (only one use count)
        EXPECT_EQ(_Shared0.use_count(), 1);
    }

    TEST(shared_array, reset) {
        // test reset functionality of shared_array
        size_t _Size              = 8192;
        shared_array<int> _Shared = ::mjx::make_shared_array<int>(_Size);
        EXPECT_NE(_Shared.get(), nullptr);
        EXPECT_EQ(_Shared.size(), _Size);
        EXPECT_EQ(_Shared.use_count(), 1);

        // calling reset() should destroy the managed array and reset the counter
        _Shared.reset();
        EXPECT_EQ(_Shared.get(), nullptr);
        EXPECT_EQ(_Shared.size(), 0);
        EXPECT_EQ(_Shared.use_count(), 0);

        // resetting with a new raw pointer should make shared_array managed the new array
        _Size           = 16384;
        int* const _Ptr = ::mjx::create_object_array<int>(_Size);
        _Shared.reset(_Ptr, _Size); // destroys _Ptr
        EXPECT_EQ(_Shared.get(), _Ptr);
        EXPECT_EQ(_Shared.size(), _Size);
        EXPECT_EQ(_Shared.use_count(), 1);
    }

    TEST(shared_array, swap) {
        // test swap functionality of shared_array
        constexpr size_t _Size0 = 32768;
        constexpr size_t _Size1 = 65536;
        int* const _Ptr0        = ::mjx::create_object_array<int>(_Size0);
        int* const _Ptr1        = ::mjx::create_object_array<int>(_Size1);
        shared_array<int> _Shared0(_Ptr0, _Size0);
        shared_array<int> _Shared1(_Ptr1, _Size1);
        _Shared0.swap(_Shared1);
        EXPECT_EQ(_Shared0.get(), _Ptr1);
        EXPECT_EQ(_Shared0.size(), _Size1);
        EXPECT_EQ(_Shared1.get(), _Ptr0);
        EXPECT_EQ(_Shared1.size(), _Size0);
    }

    TEST(shared_array, bounds_checking) {
        // test bounds checking for shared_array
        const shared_array<int> _Shared = ::mjx::make_shared_array<int>(131072);
        bool _Caught                    = false;
        try {
            (void) _Shared[_Shared.size()]; // intentionally raise resource_overrun exception
        } catch (const resource_overrun&) {
            _Caught = true;
        }

        EXPECT_TRUE(_Caught);
    }

    struct _Object_type {
        int _Ix;
        char _Cx;
        float _Fx;

        static constexpr int _Default_ix   = 610;
        static constexpr char _Default_cx  = 'U';
        static constexpr float _Default_fx = 15.022f;

        _Object_type() noexcept : _Ix(_Default_ix), _Cx(_Default_cx), _Fx(_Default_fx) {}

        explicit _Object_type(const int _Ix, const char _Cx, const float _Fx) noexcept
            : _Ix(_Ix), _Cx(_Cx), _Fx(_Fx) {}
    };

    TEST(shared_array, make_shared_array_default) {
        // test whether _Object_type array is constructed with the default values
        const shared_array<_Object_type> _Shared = ::mjx::make_shared_array<_Object_type>(64);
        for (size_t _Idx = 0; _Idx < _Shared.size(); ++_Idx) {
            EXPECT_EQ(_Shared[_Idx]._Ix, _Object_type::_Default_ix);
            EXPECT_EQ(_Shared[_Idx]._Cx, _Object_type::_Default_cx);
            EXPECT_EQ(_Shared[_Idx]._Fx, _Object_type::_Default_fx);
        }
    }

    TEST(shared_array, make_shared_array_value) {
        // test whether _Object_type array is constructed with the given values
        constexpr int _Ix                        = 527;
        constexpr char _Cx                       = 'Y';
        constexpr float _Fx                      = 924.002f;
        const shared_array<_Object_type> _Shared = ::mjx::make_shared_array<_Object_type>(
            128, _Object_type{_Ix, _Cx, _Fx});
        for (size_t _Idx = 0; _Idx < _Shared.size(); ++_Idx) {
            EXPECT_EQ(_Shared[_Idx]._Ix, _Ix);
            EXPECT_EQ(_Shared[_Idx]._Cx, _Cx);
            EXPECT_EQ(_Shared[_Idx]._Fx, _Fx);
        }
    }
} // namespace mjx