// test.cpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#include <gtest/gtest.h>
#include <mjxsdk/memory/smart_pointer.hpp>

namespace mjx {
    struct _Incomplete_type;

    struct _Non_empty_type {
        int _Obj;

        void _Func() const noexcept {}
    };

    TEST(shared_ptr, valid_element_type) {
        // test valid element types for shared_ptr
        EXPECT_TRUE(mjxsdk_impl::_Valid_smart_ptr_element<bool>);
        EXPECT_TRUE(mjxsdk_impl::_Valid_smart_ptr_element<const char>);
        EXPECT_TRUE(mjxsdk_impl::_Valid_smart_ptr_element<volatile short>);
        EXPECT_TRUE(mjxsdk_impl::_Valid_smart_ptr_element<const volatile int>);
        EXPECT_TRUE(mjxsdk_impl::_Valid_smart_ptr_element<void*>);
        EXPECT_TRUE(mjxsdk_impl::_Valid_smart_ptr_element<void* const>);
        EXPECT_TRUE(mjxsdk_impl::_Valid_smart_ptr_element<const long*>);
        EXPECT_TRUE(mjxsdk_impl::_Valid_smart_ptr_element<const long* const>);
        EXPECT_TRUE(mjxsdk_impl::_Valid_smart_ptr_element<volatile long long*>);
        EXPECT_TRUE(mjxsdk_impl::_Valid_smart_ptr_element<volatile long long* const>);
        EXPECT_TRUE(mjxsdk_impl::_Valid_smart_ptr_element<const volatile float*>);
        EXPECT_TRUE(mjxsdk_impl::_Valid_smart_ptr_element<const volatile float* const>);
        EXPECT_TRUE(mjxsdk_impl::_Valid_smart_ptr_element<double(*)(void*)>);
        EXPECT_TRUE(mjxsdk_impl::_Valid_smart_ptr_element<long double(*)[]>);
        EXPECT_TRUE(mjxsdk_impl::_Valid_smart_ptr_element<int _Non_empty_type::*>);
        EXPECT_TRUE(mjxsdk_impl::_Valid_smart_ptr_element<void(_Non_empty_type::*)()>);
    }

    TEST(shared_ptr, invalid_element_type) {
        // test invalid element types for shared_ptr
        EXPECT_FALSE(mjxsdk_impl::_Valid_smart_ptr_element<void>);
        EXPECT_FALSE(mjxsdk_impl::_Valid_smart_ptr_element<bool&>);
        EXPECT_FALSE(mjxsdk_impl::_Valid_smart_ptr_element<const char&>);
        EXPECT_FALSE(mjxsdk_impl::_Valid_smart_ptr_element<volatile short&>);
        EXPECT_FALSE(mjxsdk_impl::_Valid_smart_ptr_element<const volatile int&>);
        EXPECT_FALSE(mjxsdk_impl::_Valid_smart_ptr_element<long[]>);
        EXPECT_FALSE(mjxsdk_impl::_Valid_smart_ptr_element<long long[4]>);
        EXPECT_FALSE(mjxsdk_impl::_Valid_smart_ptr_element<float()>);
        EXPECT_FALSE(mjxsdk_impl::_Valid_smart_ptr_element<double(long double)>);
        EXPECT_FALSE(mjxsdk_impl::_Valid_smart_ptr_element<_Incomplete_type>);
    }

    TEST(shared_ptr, null_construct) {
        // test default and null construction of shared_ptr
        const shared_ptr<int> _Shared0;
        const shared_ptr<int> _Shared1(nullptr);
        EXPECT_EQ(_Shared0.get(), nullptr);
        EXPECT_EQ(_Shared0.use_count(), 0);
        EXPECT_EQ(_Shared1.get(), nullptr);
        EXPECT_EQ(_Shared1.use_count(), 0);
    }

    TEST(shared_ptr, copy_construct) {
        // test construction of shared_ptr from another instance
        const shared_ptr<int> _Shared0 = ::mjx::make_shared<int>(64);
        const shared_ptr<int> _Shared1 = _Shared0;
        EXPECT_EQ(_Shared0.get(), _Shared1.get());
        EXPECT_EQ(_Shared0.use_count(), _Shared1.use_count());
    }

    TEST(shared_ptr, move_construct) {
        // test construction of shared_ptr from another instance
        shared_ptr<int> _Shared0       = ::mjx::make_shared<int>(128);
        const shared_ptr<int> _Shared1 = ::std::move(_Shared0);
        EXPECT_EQ(_Shared0.get(), nullptr);
        EXPECT_EQ(_Shared0.use_count(), 0);
        EXPECT_NE(_Shared1.get(), nullptr);
        EXPECT_EQ(_Shared1.use_count(), 1);
    }

    TEST(shared_ptr, pointer_construct) {
        // test construction of shared_ptr from a raw pointer
        int* const _Ptr = ::mjx::create_object<int>(256);
        const shared_ptr<int> _Shared(_Ptr); // destroys _Ptr
        EXPECT_EQ(_Shared.get(), _Ptr);
        EXPECT_EQ(_Shared.use_count(), 1);
    }

    TEST(shared_ptr, unique_construct) {
        // test construction of shared_ptr from unique_ptr
        unique_ptr<int> _Unique       = ::mjx::make_unique<int>(512);
        const int* const _Ptr         = _Unique.get();
        const shared_ptr<int> _Shared = ::std::move(_Unique);
        EXPECT_EQ(_Unique.get(), nullptr);
        EXPECT_EQ(_Shared.get(), _Ptr);
        EXPECT_EQ(_Shared.use_count(), 1);
    }

    TEST(shared_ptr, copy_assign) {
        // test copy assignment of shared_ptr
        const shared_ptr<int> _Shared0 = ::mjx::make_shared<int>(1024);
        shared_ptr<int> _Shared1;
        _Shared1 = _Shared0;
        EXPECT_EQ(_Shared0.get(), _Shared1.get());
        EXPECT_EQ(_Shared0.use_count(), 2);
        EXPECT_EQ(_Shared1.use_count(), 2);
    }

    TEST(shared_ptr, move_assign) {
        // test move assignment of shared_ptr
        shared_ptr<int> _Shared0 = ::mjx::make_shared<int>(2048);
        shared_ptr<int> _Shared1;
        _Shared1 = ::std::move(_Shared0);
        EXPECT_EQ(_Shared0.get(), nullptr);
        EXPECT_EQ(_Shared0.use_count(), 0);
        EXPECT_NE(_Shared1.get(), nullptr);
        EXPECT_EQ(_Shared1.use_count(), 1);
    }

    TEST(shared_ptr, unique_assign) {
        // test assignment of unique_ptr to shared_ptr
        unique_ptr<int> _Unique = ::mjx::make_unique<int>(4096);
        shared_ptr<int> _Shared;
        _Shared = ::std::move(_Unique);
        EXPECT_EQ(_Unique.get(), nullptr);
        EXPECT_NE(_Shared.get(), nullptr);
        EXPECT_EQ(_Shared.use_count(), 1);
    }

    TEST(shared_ptr, dereference) {
        // test dereference of shared_ptr to access managed object
        constexpr int _Value          = 8192;
        const shared_ptr<int> _Shared = ::mjx::make_shared<int>(_Value);
        EXPECT_EQ(*_Shared, _Value);
    }

    TEST(shared_ptr, get) {
        // test access to the managed pointer of shared_ptr
        int* const _Ptr = ::mjx::create_object<int>(16384);
        const shared_ptr<int> _Shared(_Ptr); // destroys _Ptr
        EXPECT_EQ(_Shared.get(), _Ptr);
    }

    TEST(shared_ptr, use_count) {
        // test managed object accesss counting of shared_ptr
        const shared_ptr<int> _Shared0 = ::mjx::make_shared<int>(32768);
        EXPECT_EQ(_Shared0.use_count(), 1);
        {
            const shared_ptr<int> _Shared1 = _Shared0;
            const shared_ptr<int> _Shared2 = _Shared0;
            EXPECT_EQ(_Shared0.use_count(), 3);
        }

        // once _Shared1 and _Shared2 are destroyed, _Shared0 is unique (only one use count)
        EXPECT_EQ(_Shared0.use_count(), 1);
    }

    TEST(shared_ptr, reset) {
        // test reset functionality of shared_ptr
        shared_ptr<int> _Shared = ::mjx::make_shared<int>(131072);
        EXPECT_NE(_Shared.get(), nullptr);
        EXPECT_EQ(_Shared.use_count(), 1);

        // calling reset() should destroy the managed object and reset the counter
        _Shared.reset();
        EXPECT_EQ(_Shared.get(), nullptr);
        EXPECT_EQ(_Shared.use_count(), 0);

        // resetting with a new raw pointer should make shared_ptr manage the new object
        int* const _Ptr = ::mjx::create_object<int>(262144);
        _Shared.reset(_Ptr); // destroys _Ptr
        EXPECT_EQ(_Shared.get(), _Ptr);
        EXPECT_EQ(_Shared.use_count(), 1);
    }

    TEST(shared_ptr, swap) {
        // test swap functionality of shared_ptr
        int* const _Ptr0 = ::mjx::create_object<int>();
        int* const _Ptr1 = ::mjx::create_object<int>();
        shared_ptr<int> _Shared0(_Ptr0);
        shared_ptr<int> _Shared1(_Ptr1);
        _Shared0.swap(_Shared1);
        EXPECT_EQ(_Shared0.get(), _Ptr1);
        EXPECT_EQ(_Shared1.get(), _Ptr0);
    }

    struct _Object_type {
        int _Ix;
        char _Cx;
        float _Fx;

        static constexpr int _Default_ix   = 972;
        static constexpr char _Default_cx  = 'K';
        static constexpr float _Default_fx = 602.11f;

        _Object_type() noexcept : _Ix(_Default_ix), _Cx(_Default_cx), _Fx(_Default_fx) {}

        explicit _Object_type(const int _Ix, const char _Cx, const float _Fx) noexcept
            : _Ix(_Ix), _Cx(_Cx), _Fx(_Fx) {}
    };

    TEST(shared_ptr, make_shared_values) {
        // test whether _Object_type is constructed with the given values
        constexpr int _Ix                      = 910;
        constexpr char _Cx                     = 'T';
        constexpr float _Fx                    = 1672.02f;
        const shared_ptr<_Object_type> _Shared = ::mjx::make_shared<_Object_type>(_Ix, _Cx, _Fx);
        const _Object_type& _Obj               = *_Shared;
        EXPECT_EQ(_Obj._Ix, _Ix);
        EXPECT_EQ(_Obj._Cx, _Cx);
        EXPECT_EQ(_Obj._Fx, _Fx);
    }

    TEST(shared_ptr, make_shared_copy) {
        // test whether _Object_type is copy-constructed with the given value
        constexpr int _Ix                      = 81;
        constexpr char _Cx                     = 'L';
        constexpr float _Fx                    = 710.99f;
        const shared_ptr<_Object_type> _Shared = ::mjx::make_shared<_Object_type>(_Object_type{_Ix, _Cx, _Fx});
        const _Object_type& _Obj               = *_Shared;
        EXPECT_EQ(_Obj._Ix, _Ix);
        EXPECT_EQ(_Obj._Cx, _Cx);
        EXPECT_EQ(_Obj._Fx, _Fx);
    }

    TEST(shared_ptr, make_shared_for_overwrite) {
        // test whether _Object_type is default-constructed
        const shared_ptr<_Object_type> _Shared = ::mjx::make_shared_for_overwrite<_Object_type>();
        const _Object_type& _Obj               = *_Shared;
        EXPECT_EQ(_Obj._Ix, _Object_type::_Default_ix);
        EXPECT_EQ(_Obj._Cx, _Object_type::_Default_cx);
        EXPECT_EQ(_Obj._Fx, _Object_type::_Default_fx);
    }
} // namespace mjx