// test.cpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#include <gtest/gtest.h>
#include <mjxsdk/memory/system_allocator.hpp>

namespace mjx {
    TEST(system_allocator, aligned_allocation) {
        constexpr size_t _Count = 1000;
        constexpr size_t _Align = 64;
        system_allocator _Al;
        void* const _Ptr           = _Al.allocate(_Count, _Align);
        const uintptr_t _Ptr_bytes = reinterpret_cast<uintptr_t>(_Ptr);
        EXPECT_EQ(_Ptr_bytes % _Align, 0); // address should also be aligned
        _Al.deallocate(_Ptr, _Count, _Align);
    }

    TEST(system_allocator, tag) {
        system_allocator _Al;
        EXPECT_EQ(_Al.tag(), allocator_tag::system);
    }

    TEST(system_allocator, max_size) {
        system_allocator _Al;
#ifdef _MJX_X64
        EXPECT_EQ(_Al.max_size(), 0xFFFF'FFFF'FFFF'FFFF);
#else // ^^^ _MJX_X64 ^^^ / vvv _MJX_X86 vvv
        EXPECT_EQ(_Al.max_size(), 0xFFFF'FFFF);
#endif // _MJX_X64
    }

    TEST(system_allocator, is_equal) {
        system_allocator _Al0;
        system_allocator _Al1;
        EXPECT_EQ(_Al0, _Al1);
    }
} // namespace mjx