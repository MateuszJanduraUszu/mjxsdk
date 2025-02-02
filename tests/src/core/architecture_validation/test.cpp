// test.cpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#include <cstddef>
#include <gtest/gtest.h>

TEST(architecture_validation, pointer_size) {
    // check pointer size based on the architecture (either 64-bit or 32-bit)
#ifdef _MJX_X64
    EXPECT_EQ(sizeof(void*), 8);
#else // ^^^ _MJX_X64 ^^^ / vvv _MJX_X86 vvv
    EXPECT_EQ(sizeof(void*), 4);
#endif // _MJX_X64
}

TEST(architecture_validation, integers_size) {
    // check integers size based on the architecture (either 64-bit or 32-bit)
#ifdef _MJX_X64
    EXPECT_EQ(sizeof(intptr_t), 8);
    EXPECT_EQ(sizeof(ptrdiff_t), 8);
    EXPECT_EQ(sizeof(size_t), 8);
    EXPECT_EQ(sizeof(uintptr_t), 8);
#else // ^^^ _MJX_X64 ^^^ / vvv _MJX_X86 vvv
    EXPECT_EQ(sizeof(intptr_t), 4);
    EXPECT_EQ(sizeof(ptrdiff_t), 4);
    EXPECT_EQ(sizeof(size_t), 4);
    EXPECT_EQ(sizeof(uintptr_t), 4);
#endif // _MJX_X64
}