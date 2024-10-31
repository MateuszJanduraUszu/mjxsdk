// test.cpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#include <gtest/gtest.h>
#include <mjxsdk/smart_pointer.hpp>

namespace mjx {
    TEST(unique_ptr, null_construct) {
        unique_ptr<int> _Unique0;
        unique_ptr<int> _Unique1(nullptr);
        EXPECT_EQ(_Unique0.get(), nullptr);
        EXPECT_EQ(_Unique1.get(), nullptr);
    }

    TEST(unique_ptr, move_construct) {
        constexpr int _Value     = 32;
        unique_ptr<int> _Unique0 = ::mjx::make_unique<int>(_Value);
        unique_ptr<int> _Unique1 = ::std::move(_Unique0);
        EXPECT_EQ(_Unique0.get(), nullptr);
        EXPECT_EQ(*_Unique1, _Value);
    }

    TEST(unique_ptr, pointer_construct) {
        int* const _Ptr = ::mjx::create_object<int>(64);
        unique_ptr<int> _Unique(_Ptr); // destroys _Ptr
        EXPECT_EQ(_Ptr, _Unique.get());
        EXPECT_EQ(*_Unique, *_Ptr);
    }

    TEST(unique_ptr, null_assign) {
        unique_ptr<int> _Unique = ::mjx::make_unique<int>(128);
        _Unique                 = nullptr;
        EXPECT_EQ(_Unique.get(), nullptr);
    }

    TEST(unique_ptr, move_assign) {
        constexpr int _Value     = 512;
        unique_ptr<int> _Unique0 = ::mjx::make_unique<int>(_Value);
        unique_ptr<int> _Unique1;
        _Unique1 = ::std::move(_Unique0);
        EXPECT_EQ(_Unique0.get(), nullptr);
        EXPECT_EQ(*_Unique1, _Value);
    }

    TEST(unique_ptr, dereference) {
        int* const _Ptr = ::mjx::create_object<int>(1024);
        unique_ptr<int> _Unique(_Ptr); // destroys _Ptr
        EXPECT_EQ(*_Unique, *_Ptr);
    }

    TEST(unique_ptr, get) {
        int* const _Ptr = ::mjx::create_object<int>(2048);
        unique_ptr<int> _Unique(_Ptr); // destroys _Ptr
        EXPECT_EQ(_Unique.get(), _Ptr);
    }

    TEST(unique_ptr, release) {
        constexpr int _Value    = 4096;
        unique_ptr<int> _Unique = ::mjx::make_unique<int>(_Value);
        int* const _Ptr         = _Unique.release();
        EXPECT_EQ(_Unique.get(), nullptr);
        EXPECT_EQ(*_Ptr, _Value);
    }

    TEST(unique_ptr, reset) {
        constexpr int _Value    = 8192;
        unique_ptr<int> _Unique = ::mjx::make_unique<int>(_Value);
        EXPECT_EQ(*_Unique, _Value);
        _Unique.reset();
        EXPECT_EQ(_Unique.get(), nullptr);
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
        const unique_ptr<_Object_type> _Unique = ::mjx::make_unique<_Object_type>(25, 'Z', 99.052f);
        const _Object_type& _Obj               = *_Unique;
        EXPECT_EQ(_Obj._Ix, 25);
        EXPECT_EQ(_Obj._Cx, 'Z');
        EXPECT_EQ(_Obj._Fx, 99.052f);
    }

    TEST(unique_ptr, make_unique_for_overwrite) {
        const unique_ptr<_Object_type> _Unique = ::mjx::make_unique_for_overwrite<_Object_type>();
        const _Object_type& _Obj               = *_Unique;
        EXPECT_EQ(_Obj._Ix, 56);
        EXPECT_EQ(_Obj._Cx, 'X');
        EXPECT_EQ(_Obj._Fx, 12.062f);
    }
} // namespace mjx