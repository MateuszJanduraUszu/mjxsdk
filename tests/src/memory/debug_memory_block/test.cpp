// test.cpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#ifdef  _DEBUG
#include <gtest/gtest.h>
#include <mjxsdk/memory/impl/debug_memory_block.hpp>
#include <mjxsdk/memory/smart_pointer.hpp>

namespace mjx {
    void _Test_block_size(
        const size_t _User_size, const size_t _Align, const size_t _Block_size) noexcept {
        // verifies that the calculated block size matches the expected result
        EXPECT_EQ(mjxsdk_impl::_Calculate_block_size(_User_size, _Align), _Block_size);
    }

    void _Test_prepare_block(const size_t _Size, const size_t _Align, const allocator_tag _Tag) {
        // prepares a memory block and verifies its metadata integrity
        const size_t _Block_size = mjxsdk_impl::_Calculate_block_size(_Size, _Align);
        auto _Block              = ::mjx::make_unique_array<unsigned char>(_Block_size);
        mjxsdk_impl::_Prepare_block(_Block.get(), _Block_size, _Size, _Align, _Tag);

        const auto _Meta = mjxsdk_impl::_Extract_metadata(_Block.get(), _Size, _Align);
        EXPECT_EQ(_Meta._Header._Size, _Size);
        EXPECT_EQ(_Meta._Header._Align, _Align);
        EXPECT_EQ(_Meta._Header._Tag, _Tag);
        EXPECT_EQ(_Meta._Header._State, mjxsdk_impl::_Block_state::_Allocated);
    }

    TEST(debug_memory_block, block_size) {
#ifdef _MJX_X64
        _Test_block_size(128, 2, 154);
        _Test_block_size(4096, 8, 4128);
#else // ^^^ _MJX_X64 ^^^ / vvv _MJX_X86 vvv
        _Test_block_size(128, 2, 146);
        _Test_block_size(4096, 8, 4120);
#endif // _MJX_X64

        // these are equal on both x64 and x86 due to high alignment
        _Test_block_size(0x0000'FFFF, 32, 0x0001'0040);
        _Test_block_size(0x1000'0000, 128, 0x1000'0100);
        _Test_block_size(0xFFFF'0000, 512, 0xFFFF'0400);
    }

    TEST(debug_memory_block, prepare_block) {
        _Test_prepare_block(37, 1, allocator_tag{20});
        _Test_prepare_block(64, 32, allocator_tag{52});
        _Test_prepare_block(511, 128, allocator_tag{143});
        _Test_prepare_block(4033, 512, allocator_tag{212});
        _Test_prepare_block(16384, 2048, allocator_tag{255});
    }
} // namespace mjx
#endif // _DEBUG