// test.cpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#include <gtest/gtest.h>
#include <mjxsdk/endian.hpp>

#ifdef _MJX_INT128_SUPPORTED
// since 128-bit literals aren't supported, manually merge the lower and upper bits
#define _MAKE_UINT128(_Low, _High) static_cast<uint128_t>(_Low | (static_cast<uint128_t>(_High) << 64))
#endif // _MJX_INT128_SUPPORTED

namespace mjx {
    TEST(endian, swap_endian_16) {
        EXPECT_EQ(::mjx::swap_endian(uint16_t{0x1A2B}), 0x2B1A);
        EXPECT_EQ(::mjx::swap_endian(uint16_t{0x3C4D}), 0x4D3C);
        EXPECT_EQ(::mjx::swap_endian(uint16_t{0x5E6F}), 0x6F5E);
        EXPECT_EQ(::mjx::swap_endian(uint16_t{0x7F8A}), 0x8A7F);
        EXPECT_EQ(::mjx::swap_endian(uint16_t{0x9BCD}), 0xCD9B);
    }

    TEST(endian, swap_endian_32) {
        EXPECT_EQ(::mjx::swap_endian(uint32_t{0x1234'5678}), 0x7856'3412);
        EXPECT_EQ(::mjx::swap_endian(uint32_t{0xA1B2'C3D4}), 0xD4C3'B2A1);
        EXPECT_EQ(::mjx::swap_endian(uint32_t{0x89AB'CDEF}), 0xEFCD'AB89);
        EXPECT_EQ(::mjx::swap_endian(uint32_t{0x0F1E'2D3C}), 0x3C2D'1E0F);
        EXPECT_EQ(::mjx::swap_endian(uint32_t{0x5566'7788}), 0x8877'6655);
    }

    TEST(endian, swap_endian_64) {
        EXPECT_EQ(::mjx::swap_endian(uint64_t{0x1234'5678'90AB'CDEF}), 0xEFCD'AB90'7856'3412);
        EXPECT_EQ(::mjx::swap_endian(uint64_t{0xA1B2'C3D4'E5F6'0789}), 0x8907'F6E5'D4C3'B2A1);
        EXPECT_EQ(::mjx::swap_endian(uint64_t{0x89AB'CDEF'0123'4567}), 0x6745'2301'EFCD'AB89);
        EXPECT_EQ(::mjx::swap_endian(uint64_t{0x0F1E'2D3C'4B5A'6978}), 0x7869'5A4B'3C2D'1E0F);
        EXPECT_EQ(::mjx::swap_endian(uint64_t{0x5566'7788'99AA'BBCC}), 0xCCBB'AA99'8877'6655);
    }

#ifdef _MJX_INT128_SUPPORTED
    TEST(endian, swap_endian_128) {
        EXPECT_EQ(::mjx::swap_endian(_MAKE_UINT128(0x1234'5678'9ABC'DEF0, 0x1234'5678'9ABC'DEF0)),
            _MAKE_UINT128(0xF0DE'BC9A'7856'3412, 0xF0DE'BC9A'7856'3412));
        EXPECT_EQ(::mjx::swap_endian(_MAKE_UINT128(0xA1B2'C3D4'E5F6'0789, 0xA1B2'C3D4'E5F6'0789)),
            _MAKE_UINT128(0x8907'F6E5'D4C3'B2A1, 0x8907'F6E5'D4C3'B2A1));
        EXPECT_EQ(::mjx::swap_endian(_MAKE_UINT128(0x89AB'CDEF'0123'4567, 0x89AB'CDEF'0123'4567)),
            _MAKE_UINT128(0x6745'2301'EFCD'AB89, 0x6745'2301'EFCD'AB89));
        EXPECT_EQ(::mjx::swap_endian(_MAKE_UINT128(0x0F1E'2D3C'4B5A'6978, 0x0F1E'2D3C'4B5A'6978)),
            _MAKE_UINT128(0x7869'5A4B'3C2D'1E0F, 0x7869'5A4B'3C2D'1E0F));
        EXPECT_EQ(::mjx::swap_endian(_MAKE_UINT128(0x5566'7788'99AA'BBCC, 0x5566'7788'99AA'BBCC)),
            _MAKE_UINT128(0xCCBB'AA99'8877'6655, 0xCCBB'AA99'8877'6655));
    }
#endif // _MJX_INT128_SUPPORTED

    template <integral _Ty>
    void test_to_endian(const _Ty _Value) noexcept {
        const _Ty _Swapped = ::mjx::swap_endian(_Value);
#ifdef _MJX_LITTLE_ENDIAN
        EXPECT_EQ(::mjx::to_little_endian(_Value), _Value);
        EXPECT_EQ(::mjx::to_big_endian(_Value), _Swapped);
#else // ^^^ _MJX_LITTLE_ENDIAN ^^^ / vvv _MJX_BIG_ENDIAN vvv
        EXPECT_EQ(::mjx::to_little_endian(_Value), _Swapped);
        EXPECT_EQ(::mjx::to_big_endian(_Value), _Value);
#endif // _MJX_LITTLE_ENDIAN
    }

    template <integral _Ty>
    void test_from_endian(const _Ty _Value) noexcept {
        const _Ty _Swapped = ::mjx::swap_endian(_Value);
#ifdef _MJX_LITTLE_ENDIAN
        EXPECT_EQ(::mjx::from_little_endian(_Value), _Value);
        EXPECT_EQ(::mjx::from_big_endian(_Value), _Swapped);
#else // ^^^ _MJX_LITTLE_ENDIAN ^^^ / vvv _MJX_BIG_ENDIAN vvv
        EXPECT_EQ(::mjx::from_little_endian(_Value), _Swapped);
        EXPECT_EQ(::mjx::from_big_endian(_Value), _Value);
#endif // _MJX_LITTLE_ENDIAN
    }

    TEST(endian, to_endian) {
        test_to_endian(uint16_t{0x1A2B});
        test_to_endian(uint32_t{0x1234'5678});
        test_to_endian(uint64_t{0x1234'5678'9ABC'DEF0});
#ifdef _MJX_INT128_SUPPORTED
        test_to_endian(_MAKE_UINT128(0xBEEF'CAFE'1234'5678, 0x0FED'CBA9'8765'4321));
#endif // _MJX_INT128_SUPPORTED
    }

    TEST(endian, from_endian) {
        test_from_endian(uint16_t{0x3C4D});
        test_from_endian(uint32_t{0x9ABC'DEF0});
        test_from_endian(uint64_t{0x0F1E'2D3C'4B5A'6978});
#ifdef _MJX_INT128_SUPPORTED
        test_from_endian(_MAKE_UINT128(0x1234'5678'9ABC'DEF0, 0xA1B2'C3D4'E5F6'7890));
#endif // _MJX_INT128_SUPPORTED
    }
} // namespace mjx