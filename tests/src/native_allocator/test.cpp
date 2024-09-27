// test.cpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#include <gtest/gtest.h>
#include <mjxsdk/native_allocator.hpp>

namespace mjx {
    TEST(native_allocator, max_size) {
        native_allocator _Al;
#ifdef _MJX_X64
        EXPECT_EQ(_Al.max_size(), 0xFFFF'FFFF'FFFF'FFFF);
#else // ^^^ _MJX_X64 ^^^ / vvv _MJX_X86 vvv
        EXPECT_EQ(_Al.max_size(), 0xFFFF'FFFF);
#endif // _MJX_X64
    }

    TEST(native_allocator, is_equal) {
        native_allocator _Al0;
        native_allocator _Al1;
        EXPECT_EQ(_Al0, _Al1);
    }
} // namespace mjx