// test.cpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#include <gtest/gtest.h>
#include <mjxsdk/smart_pointer.hpp>

namespace mjx {
    TEST(unique_ptr, null_construct) {
        // test default and null construction of unique_ptr
        const unique_ptr<int> _Unique0;
        const unique_ptr<int> _Unique1(nullptr);
        EXPECT_EQ(_Unique0.get(), nullptr);
        EXPECT_EQ(_Unique1.get(), nullptr);
    }

    TEST(unique_ptr, move_construct) {
        // test construction of unique_ptr from another instance
        unique_ptr<int> _Unique0       = ::mjx::make_unique<int>(32);
        const int* const _Ptr          = _Unique0.get();
        const unique_ptr<int> _Unique1 = ::std::move(_Unique0);
        EXPECT_EQ(_Unique0.get(), nullptr);
        EXPECT_EQ(_Unique1.get(), _Ptr);
    }

    TEST(unique_ptr, pointer_construct) {
        // test construction of unique_ptr from a raw pointer
        int* const _Ptr = ::mjx::create_object<int>(64);
        const unique_ptr<int> _Unique(_Ptr); // destroys _Ptr
        EXPECT_EQ(_Unique.get(), _Ptr);
    }

    TEST(unique_ptr, null_assign) {
        // test assignment of null to unique_ptr
        unique_ptr<int> _Unique = ::mjx::make_unique<int>(128);
        _Unique                 = nullptr;
        EXPECT_EQ(_Unique.get(), nullptr);
    }

    TEST(unique_ptr, move_assign) {
        // test move assignment of unique_ptr
        unique_ptr<int> _Unique0 = ::mjx::make_unique<int>(512);
        const int* const _Ptr    = _Unique0.get();
        unique_ptr<int> _Unique1;
        _Unique1 = ::std::move(_Unique0);
        EXPECT_EQ(_Unique0.get(), nullptr);
        EXPECT_EQ(_Unique1.get(), _Ptr);
    }

    TEST(unique_ptr, dereference) {
        // test dereference of unique_ptr to access managed object
        constexpr int _Value          = 1024;
        const unique_ptr<int> _Unique = ::mjx::make_unique<int>(_Value);
        EXPECT_EQ(*_Unique, _Value);
    }

    TEST(unique_ptr, get) {
        // test access to the managed object of unique_ptr
        int* const _Ptr = ::mjx::create_object<int>(2048);
        unique_ptr<int> _Unique(_Ptr); // destroys _Ptr
        EXPECT_EQ(_Unique.get(), _Ptr);
    }

    TEST(unique_ptr, release) {
        // test release of unique_ptr, transferring ownership
        int* const _Ptr = ::mjx::create_object<int>(4096);
        unique_ptr<int> _Unique(_Ptr);
        const int* const _Released_ptr = _Unique.release();
        EXPECT_EQ(_Unique.get(), nullptr);
        EXPECT_EQ(_Released_ptr, _Ptr);
        ::mjx::delete_object(_Ptr);
    }

    TEST(unique_ptr, reset) {
        // test reset of unique_ptr, deleting the managed object
        int* const _Ptr = ::mjx::create_object<int>(8192);
        unique_ptr<int> _Unique(_Ptr);
        EXPECT_EQ(_Unique.get(), _Ptr);

        // calling reset() should destroy the managed object
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

        static constexpr int _Default_ix   = 56;
        static constexpr char _Default_cx  = 'X';
        static constexpr float _Default_fx = 12.062f;

        _Object_type() noexcept : _Ix(_Default_ix), _Cx(_Default_cx), _Fx(_Default_fx) {}

        explicit _Object_type(const int _Ix, const char _Cx, const float _Fx) noexcept
            : _Ix(_Ix), _Cx(_Cx), _Fx(_Fx) {}
    };

    TEST(unique_ptr, make_unique_values) {
        // test whether _Object_type is constructed with the given values
        constexpr int _Ix                      = 10;
        constexpr char _Cx                     = 'C';
        constexpr float _Fx                    = 71.122f;
        const unique_ptr<_Object_type> _Unique = ::mjx::make_unique<_Object_type>(_Ix, _Cx, _Fx);
        const _Object_type& _Obj               = *_Unique;
        EXPECT_EQ(_Obj._Ix, _Ix);
        EXPECT_EQ(_Obj._Cx, _Cx);
        EXPECT_EQ(_Obj._Fx, _Fx);
    }

    TEST(unique_ptr, make_unique_copy) {
        // test whether _Object_type is copy-constructed with the given value
        constexpr int _Ix                      = 1925;
        constexpr char _Cx                     = 'P';
        constexpr float _Fx                    = 5290.251f;
        const unique_ptr<_Object_type> _Unique = ::mjx::make_unique<_Object_type>(_Object_type{_Ix, _Cx, _Fx});
        const _Object_type& _Obj               = *_Unique;
        EXPECT_EQ(_Obj._Ix, _Ix);
        EXPECT_EQ(_Obj._Cx, _Cx);
        EXPECT_EQ(_Obj._Fx, _Fx);
    }

    TEST(unique_ptr, make_unique_for_overwrite) {
        // test whether _Object_type is default-constructed
        const unique_ptr<_Object_type> _Unique = ::mjx::make_unique_for_overwrite<_Object_type>();
        const _Object_type& _Obj               = *_Unique;
        EXPECT_EQ(_Obj._Ix, _Object_type::_Default_ix);
        EXPECT_EQ(_Obj._Cx, _Object_type::_Default_cx);
        EXPECT_EQ(_Obj._Fx, _Object_type::_Default_fx);
    }
} // namespace mjx