// test.cpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#include <gtest/gtest.h>
#include <mjxsdk/core/version.hpp>

namespace mjx {
    void test_encode_decode(const uint64_t _Major, const uint64_t _Minor, const uint64_t _Patch) noexcept {
        // encode the specified version into a 64-bit integer, and subsequently decode it
        // to validate the extracted version numbers
        const uint64_t _Version = _MJX_ENCODE_VERSION(_Major, _Minor, _Patch);
        EXPECT_EQ(_MJX_DECODE_VERSION_MAJOR(_Version), _Major);
        EXPECT_EQ(_MJX_DECODE_VERSION_MINOR(_Version), _Minor);
        EXPECT_EQ(_MJX_DECODE_VERSION_PATCH(_Version), _Patch);
    }

    void test_out_of_bounds(const uint64_t _Major, const uint64_t _Minor, const uint64_t _Patch) noexcept {
        // Note: This test assumes that one or more of the version numbers are out of bounds, resulting
        //       in an invalid version encoding. Even if at least one number can be successfully decoded,
        //       the test asserts that not all numbers are successfully decoded.
        const uint64_t _Version   = _MJX_ENCODE_VERSION(_Major, _Minor, _Patch);
        const bool _Decoded_major = _MJX_DECODE_VERSION_MAJOR(_Version) == _Major;
        const bool _Decoded_minor = _MJX_DECODE_VERSION_MINOR(_Version) == _Minor;
        const bool _Decoded_patch = _MJX_DECODE_VERSION_PATCH(_Version) == _Patch;
        EXPECT_FALSE(_Decoded_major && _Decoded_minor && _Decoded_patch);
    }

    TEST(version_encoding, encode_decode) {
        test_encode_decode(2, 4, 1);
        test_encode_decode(6, 7, 9);
        test_encode_decode(30, 66, 24);
        test_encode_decode(90, 80, 56);
        test_encode_decode(436, 105, 304);
        test_encode_decode(990, 584, 466);
        test_encode_decode(4187, 3660, 2886);
        test_encode_decode(7146, 9500, 6035);
        test_encode_decode(22256, 39409, 43925);
        test_encode_decode(38963, 59099, 61202);
    }

    TEST(version_encoding, out_of_bounds) {
        test_out_of_bounds(2, 15, 0xFFFF'FF01);
        test_out_of_bounds(51, 0xFFFF'FF02, 62);
        test_out_of_bounds(0x0001'0000, 734, 913);
        test_out_of_bounds(9051, 0xFFFF'FF02, 0xFFFF'FF01);
        test_out_of_bounds(0x0001'0000, 0xFFFF'FF02, 41097);
        test_out_of_bounds(0x0001'0000, 0xFFFF'FF02, 0xFFFF'FF01);
    }
} // namespace mjx