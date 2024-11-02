// test.cpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#include <gtest/gtest.h>
#include <mjxsdk/smart_pointer.hpp>

namespace mjx {
    TEST(unique_ptr, null_construct) {
        // test default and null construction of unique_ptr
        unique_ptr<int> _Unique0;
        unique_ptr<int> _Unique1(nullptr);
        EXPECT_EQ(_Unique0.get(), nullptr);
        EXPECT_EQ(_Unique1.get(), nullptr);
    }

    TEST(unique_ptr, move_construct) {
        // test construction of unique_ptr from another instance
        constexpr int _Value     = 32;
        unique_ptr<int> _Unique0 = ::mjx::make_unique<int>(_Value);
        unique_ptr<int> _Unique1 = ::std::move(_Unique0);
        EXPECT_EQ(_Unique0.get(), nullptr);
        EXPECT_EQ(*_Unique1, _Value);
    }

    TEST(unique_ptr, pointer_construct) {
        // test construction of unique_ptr from a raw pointer
        int* const _Ptr = ::mjx::create_object<int>(64);
        unique_ptr<int> _Unique(_Ptr); // destroys _Ptr
        EXPECT_EQ(_Ptr, _Unique.get());
        EXPECT_EQ(*_Unique, *_Ptr);
    }

    TEST(unique_ptr, null_assign) {
        // test assignment of null to unique_ptr
        unique_ptr<int> _Unique = ::mjx::make_unique<int>(128);
        _Unique                 = nullptr;
        EXPECT_EQ(_Unique.get(), nullptr);
    }

    TEST(unique_ptr, move_assign) {
        // test move assignment of unique_ptr
        constexpr int _Value     = 512;
        unique_ptr<int> _Unique0 = ::mjx::make_unique<int>(_Value);
        unique_ptr<int> _Unique1;
        _Unique1 = ::std::move(_Unique0);
        EXPECT_EQ(_Unique0.get(), nullptr);
        EXPECT_EQ(*_Unique1, _Value);
    }

    TEST(unique_ptr, dereference) {
        // test dereference of unique_ptr to access managed object
        int* const _Ptr = ::mjx::create_object<int>(1024);
        unique_ptr<int> _Unique(_Ptr); // destroys _Ptr
        EXPECT_EQ(*_Unique, *_Ptr);
    }

    TEST(unique_ptr, get) {
        // test access to the managed pointer of unique_ptr
        int* const _Ptr = ::mjx::create_object<int>(2048);
        unique_ptr<int> _Unique(_Ptr); // destroys _Ptr
        EXPECT_EQ(_Unique.get(), _Ptr);
    }

    TEST(unique_ptr, release) {
        // test release of unique_ptr, transferring ownership
        constexpr int _Value    = 4096;
        unique_ptr<int> _Unique = ::mjx::make_unique<int>(_Value);
        int* const _Ptr         = _Unique.release();
        EXPECT_EQ(_Unique.get(), nullptr);
        EXPECT_EQ(*_Ptr, _Value);
    }

    TEST(unique_ptr, reset) {
        // test reset of unique_ptr, deleting the managed pointer
        constexpr int _Value    = 8192;
        unique_ptr<int> _Unique = ::mjx::make_unique<int>(_Value);
        EXPECT_EQ(*_Unique, _Value);
        _Unique.reset();
        EXPECT_EQ(_Unique.get(), nullptr);
    }

    TEST(unique_ptr, swap) {
        // test swap functionality of unique_ptr
        int* const _Ptr0 = ::mjx::create_object<int>();
        int* const _Ptr1 = ::mjx::create_object<int>();
        unique_ptr<int> _Unique0(_Ptr0);
        unique_ptr<int> _Unique1(_Ptr1);
        _Unique0.swap(_Unique1);
        EXPECT_EQ(_Unique0.get(), _Ptr1);
        EXPECT_EQ(_Unique1.get(), _Ptr0);
    }

    struct _Object_type {
        int _Ix;
        char _Cx;
        float _Fx;

        _Object_type() noexcept : _Ix(56), _Cx('X'), _Fx(12.062) {}

        explicit _Object_type(const int _Ix, const char _Cx, const float _Fx) noexcept
            : _Ix(_Ix), _Cx(_Cx), _Fx(_Fx) {}
    };

    TEST(unique_ptr, make_unique) {
        // test whether _Object_type is constructed with the given values
        const unique_ptr<_Object_type> _Unique = ::mjx::make_unique<_Object_type>(25, 'Z', 99.052f);
        const _Object_type& _Obj               = *_Unique;
        EXPECT_EQ(_Obj._Ix, 25);
        EXPECT_EQ(_Obj._Cx, 'Z');
        EXPECT_EQ(_Obj._Fx, 99.052f);
    }

    TEST(unique_ptr, make_unique_for_overwrite) {
        // test whether _Object_type is default-constructed
        const unique_ptr<_Object_type> _Unique = ::mjx::make_unique_for_overwrite<_Object_type>();
        const _Object_type& _Obj               = *_Unique;
        EXPECT_EQ(_Obj._Ix, 56);
        EXPECT_EQ(_Obj._Cx, 'X');
        EXPECT_EQ(_Obj._Fx, 12.062f);
    }
} // namespace mjx