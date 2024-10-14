// test.cpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#include <gtest/gtest.h>
#include <mjxsdk/object.hpp>

namespace mjx {
    struct _Object_with_members { // an object type with members
        bool _Mem0;
        char _Mem1;
        int _Mem2;

        _Object_with_members() noexcept : _Mem0(false), _Mem1('\0'), _Mem2(0) {}

        _Object_with_members(const bool _Val0, const char _Val1, const int _Val2) noexcept
            : _Mem0(_Val0), _Mem1(_Val1), _Mem2(_Val2) {}
    };

    enum class _Object_state : int {
        _Uninitialized,
        _Constructed,
        _Destroyed
    };

    struct _Object_with_state { // an object type with state
        _Object_state* _State_ptr;

        explicit _Object_with_state(_Object_state* const _State) noexcept : _State_ptr(_State) {
            *_State_ptr = _Object_state::_Constructed;
        }

        ~_Object_with_state() noexcept {
            *_State_ptr = _Object_state::_Destroyed;
        }
    };

    struct _Object_with_state_no_dtor { // an object type with state and no destructor
        _Object_state* _State_ptr;

        explicit _Object_with_state_no_dtor(_Object_state* const _State) noexcept : _State_ptr(_State) {
            *_State_ptr = _Object_state::_Constructed;
        }
    };

    TEST(object_management, construct_and_destruct_object) {
        // test object construction and destruction based on the tracked state
        _Object_state _State                             = _Object_state::_Uninitialized;
        unsigned char _Space[sizeof(_Object_with_state)] = {0};
        ::mjx::construct_object(reinterpret_cast<_Object_with_state*>(_Space), &_State);
        EXPECT_EQ(_State, _Object_state::_Constructed);
        ::mjx::destroy_object(reinterpret_cast<_Object_with_state*>(_Space));
        EXPECT_EQ(_State, _Object_state::_Destroyed);
    }

    TEST(object_management, construct_and_destruct_object_no_dtor) {
        // test object construction and destruction based on the tracked state, where the object
        // type has no non-trivial destructor, so the state doesn't change on destruction
        _Object_state _State                                     = _Object_state::_Uninitialized;
        unsigned char _Space[sizeof(_Object_with_state_no_dtor)] = {0};
        ::mjx::construct_object(reinterpret_cast<_Object_with_state_no_dtor*>(_Space), &_State);
        EXPECT_EQ(_State, _Object_state::_Constructed);
        ::mjx::destroy_object(reinterpret_cast<_Object_with_state_no_dtor*>(_Space));
        EXPECT_EQ(_State, _Object_state::_Constructed); // no changes are expected
    }

    TEST(object_management, create_and_delete_object) {
        // test object creation and deletion based on the stored values,
        // which are initialized when the object is constructed
        _Object_with_members* const _Obj = ::mjx::create_object<_Object_with_members>(true, 'X', 1024);
        EXPECT_EQ(_Obj->_Mem0, true);
        EXPECT_EQ(_Obj->_Mem1, 'X');
        EXPECT_EQ(_Obj->_Mem2, 1024);
        ::mjx::delete_object(_Obj);
    }

    TEST(object_management, create_and_delete_object_array) {
        // test array of objects creation and deletion based on the stored values,
        // which are initialized when the objects are constructed
        constexpr size_t _Array_size       = 8;
        _Object_with_members* const _Array = ::mjx::create_object_array<_Object_with_members>(_Array_size);
        for (size_t _Idx = 0; _Idx < _Array_size; ++_Idx) {
            EXPECT_EQ(_Array[_Idx]._Mem0, false);
            EXPECT_EQ(_Array[_Idx]._Mem1, '\0');
            EXPECT_EQ(_Array[_Idx]._Mem2, 0);
        }

        ::mjx::delete_object_array(_Array, _Array_size);
    }
} // namespace mjx