// test.cpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#include <gtest/gtest.h>
#include <mjxsdk/object.hpp>

namespace mjx {
    struct _Object_base {
        bool* _Called; // pointer to a flag that stores true if non-trivial destructor has been called
    };

    struct _Trivial_object : public _Object_base {
        // trivial object with default trivial destructor
    };

    struct _Non_trivial_object : public _Object_base {
        // non-trivial object with a non-trivial (user-defined) destructor

        ~_Non_trivial_object() noexcept {
            if (_Called) {
                *_Called = true;
            }
        }
    };

    static_assert(::std::is_trivially_destructible_v<_Trivial_object>,
        "_Trivial_object must be trivially destructible");
    static_assert(!::std::is_trivially_destructible_v<_Non_trivial_object>,
        "_Non_trivial_object must not be trivially destructible");

    TEST(object_management, trivial_object_destruction) {
        // test whether a type with trivial destructor will call it (expected not to do so)
        bool _Called                = false;
        _Trivial_object* const _Obj = ::mjx::create_object<_Trivial_object>();
        _Obj->_Called               = &_Called;
        ::mjx::delete_object(_Obj);
        EXPECT_FALSE(_Called);
    }

    TEST(object_management, non_trivial_object_destruction) {
        // test whether a type with a non-trivial destructor will call it (expected to do so)
        bool _Called                    = false;
        _Non_trivial_object* const _Obj = ::mjx::create_object<_Non_trivial_object>();
        _Obj->_Called                   = &_Called;
        ::mjx::delete_object(_Obj);
        EXPECT_TRUE(_Called);
    }
} // namespace mjx