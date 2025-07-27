// utils.hpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef _MJXSDK_MEMORY_IMPL_UTILS_HPP_
#define _MJXSDK_MEMORY_IMPL_UTILS_HPP_
#include <cstddef>

namespace mjx {
    namespace mjxsdk_impl {
        template <class _OffTy>
        inline constexpr bool _Is_valid_offset_type = ::std::is_integral_v<_OffTy>
            && !::std::is_same_v<_OffTy, bool>; // allows all integral types except bool

        template <class _OffTy>
        inline void* _Adjust_address_by_offset(void* const _Address, const _OffTy _Off) noexcept {
            // returns a new address by moving the base address by the given offset
            static_assert(_Is_valid_offset_type<_OffTy>, "_OffTy must be a non-bool integral type");
            return static_cast<void*>(static_cast<unsigned char*>(_Address) + _Off);
        }

        template <class _OffTy>
        inline const void* _Adjust_address_by_offset(const void* const _Address, const _OffTy _Off) noexcept {
            // returns a new address by moving the base address by the given offset
            static_assert(_Is_valid_offset_type<_OffTy>, "_OffTy must be a non-bool integral type");
            return static_cast<const void*>(static_cast<const unsigned char*>(_Address) + _Off);
        }

        inline bool _Is_within_memory_block(const void* const _Base_begin,
            const void* const _Base_end, const void* const _Block_begin, const void* const _Block_end) noexcept {
            // checks whether the memory block is within the base memory block
            return _Block_begin >= _Base_begin && _Block_end <= _Base_end;
        }

        constexpr size_t _Get_effective_alignment(const size_t _Align) noexcept {
            // returns the effective alignment for allocation and deallocation
            return _Align >= __STDCPP_DEFAULT_NEW_ALIGNMENT__ ? _Align : __STDCPP_DEFAULT_NEW_ALIGNMENT__;
        }
    } // namespace mjxsdk_impl
} // namespace mjx

#endif // _MJXSDK_MEMORY_IMPL_UTILS_HPP_