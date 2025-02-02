// endian.hpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef _MJXSDK_MEMORY_ENDIAN_HPP_
#define _MJXSDK_MEMORY_ENDIAN_HPP_
#include <cstdint>
#include <type_traits>
#ifdef _MJX_MSVC
#include <cstdlib>
#endif // _MJX_MSVC

namespace mjx {
#ifdef _MJX_INT128_SUPPORTED
    using int128_t  = __int128;
    using uint128_t = unsigned __int128;
#endif // _MJX_INT128_SUPPORTED

    template <class _Ty>
    concept integral = ::std::disjunction_v<::std::is_integral<_Ty>
#ifdef _MJX_INT128_SUPPORTED
        , ::std::is_same<_Ty, int128_t>, ::std::is_same<_Ty, uint128_t>
#endif // _MJX_INT128_SUPPORTED
    >;

    enum class endian : unsigned char {
        little = 0,
        big    = 1,
#ifdef _MJX_LITTLE_ENDIAN
        native = little
#else // ^^^ _MJX_LITTLE_ENDIAN ^^^ / vvv _MJX_BIG_ENDIAN vvv
        native = big
#endif // _MJX_LITTLE_ENDIAN
    };
    
    // Note: The functions below should be placed in an internal header as they are
    //       part of the implementation details. However, to maintain swap_endian() constexpr,
    //       these functions must be implemented in a public header and should not be exported.
    namespace mjxsdk_impl {
        constexpr uint16_t _Bswap16(const uint16_t _Value) noexcept {
#if defined(_MJX_CLANG) || defined(_MJX_GCC)
            return __builtin_bswap16(_Value);
#else // ^^^ Clang or GCC ^^^ / vvv MSVC vvv
            if (!::std::is_constant_evaluated()) {
                return ::_byteswap_ushort(_Value);
            } else {
                return (_Value >> 8) | (_Value << 8);
            }
#endif // defined(_MJX_CLANG) || defined(_MJX_GCC)
        }

        constexpr uint32_t _Bswap32(const uint32_t _Value) noexcept {
#if defined(_MJX_CLANG) || defined(_MJX_GCC)
            return __builtin_bswap32(_Value);
#else // ^^^ Clang or GCC ^^^ / vvv MSVC vvv
            if (!::std::is_constant_evaluated()) {
                return ::_byteswap_ulong(_Value);
            } else {
                return (_Value >> 24) | ((_Value & 0x00FF'0000) >> 8)
                    | ((_Value & 0x0000'FF00) << 8) | (_Value << 24);
            }
#endif // defined(_MJX_CLANG) || defined(_MJX_GCC)
        }

        constexpr uint64_t _Bswap64(const uint64_t _Value) noexcept {
#if defined(_MJX_CLANG) || defined(_MJX_GCC)
            return __builtin_bswap64(_Value);
#else // ^^^ Clang or GCC ^^^ / vvv MSVC vvv
            if (!::std::is_constant_evaluated()) {
                return ::_byteswap_uint64(_Value);
            } else {
                return (_Value >> 56) | ((_Value & 0x00FF'0000'0000'0000) >> 40)
                    | ((_Value & 0x0000'FF00'0000'0000) >> 24) | ((_Value & 0x0000'00FF'0000'0000) >> 8)
                    | ((_Value & 0x0000'0000'FF00'0000) << 8) | ((_Value & 0x0000'0000'00FF'0000) << 24)
                    | ((_Value & 0x0000'0000'0000'FF00) << 40) | (_Value << 56);
            }
#endif // defined(_MJX_CLANG) || defined(_MJX_GCC)
        }

#ifdef _MJX_INT128_SUPPORTED
        constexpr uint128_t _Bswap128(const uint128_t _Value) noexcept {
#ifdef _MJX_GCC
            return __builtin_bswap128(_Value);
#else // ^^^ GCC ^^^ / vvv Clang or MSVC vvv
            return (static_cast<uint128_t>(
                _Bswap64(_Value & 0xFFFF'FFFF'FFFF'FFFF)) << 64) | _Bswap64(_Value >> 64);
#endif // _MJX_GCC
        }
#endif // _MJX_INT128_SUPPORTED

        template <class...>
        inline constexpr bool _Always_false = false;
    } // namespace mjxsdk_impl

    template <integral _Ty>
    constexpr _Ty swap_endian(const _Ty _Value) noexcept {
        // reverse the bytes of the given integer
        if constexpr (sizeof(_Ty) == 1) {
            return _Value;
        } else if constexpr (sizeof(_Ty) == 2) {
            return static_cast<_Ty>(mjxsdk_impl::_Bswap16(static_cast<uint16_t>(_Value)));
        } else if constexpr (sizeof(_Ty) == 4) {
            return static_cast<_Ty>(mjxsdk_impl::_Bswap32(static_cast<uint32_t>(_Value)));
        } else if constexpr (sizeof(_Ty) == 8) {
            return static_cast<_Ty>(mjxsdk_impl::_Bswap64(static_cast<uint64_t>(_Value)));
        }
#ifdef _MJX_INT128_SUPPORTED
        else if constexpr (sizeof(_Ty) == 16) {
            return static_cast<_Ty>(mjxsdk_impl::_Bswap128(static_cast<uint128_t>(_Value)));
        }
#endif // _MJX_INT128_SUPPORTED
        else {
            static_assert(mjxsdk_impl::_Always_false<_Ty>, "unexpected integral size");
        }
    }

    template <integral _Ty>
    constexpr _Ty to_little_endian(const _Ty _Value) noexcept {
#ifdef _MJX_LITTLE_ENDIAN
        return _Value;
#else // ^^^ _MJX_LITTLE_ENDIAN ^^^ / vvv _MJX_BIG_ENDIAN vvv
        return ::mjx::swap_endian(_Value);
#endif // _MJX_LITTLE_ENDIAN
    }

    template <integral _Ty>
    constexpr _Ty to_big_endian(const _Ty _Value) noexcept {
#ifdef _MJX_BIG_ENDIAN
        return _Value;
#else // ^^^ _MJX_BIG_ENDIAN ^^^ / vvv _MJX_LITTLE_ENDIAN vvv
        return ::mjx::swap_endian(_Value);
#endif // _MJX_BIG_ENDIAN
    }

    template <integral _Ty>
    constexpr _Ty from_little_endian(const _Ty _Value) noexcept {
#ifdef _MJX_LITTLE_ENDIAN
        return _Value;
#else // ^^^ _MJX_LITTLE_ENDIAN ^^^ / vvv _MJX_BIG_ENDIAN vvv
        return ::mjx::swap_endian(_Value);
#endif // _MJX_LITTLE_ENDIAN
    }

    template <integral _Ty>
    constexpr _Ty from_big_endian(const _Ty _Value) noexcept {
#ifdef _MJX_BIG_ENDIAN
        return _Value;
#else // ^^^ _MJX_BIG_ENDIAN ^^^ / vvv _MJX_LITTLE_ENDIAN vvv
        return ::mjx::swap_endian(_Value);
#endif // _MJX_BIG_ENDIAN
    }
} // namespace mjx

#endif // _MJXSDK_MEMORY_ENDIAN_HPP_