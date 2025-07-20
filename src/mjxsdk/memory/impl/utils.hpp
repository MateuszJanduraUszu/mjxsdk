// utils.hpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef _MJXSDK_MEMORY_IMPL_UTILS_HPP_
#define _MJXSDK_MEMORY_IMPL_UTILS_HPP_
#include <cstddef>

namespace mjx {
    namespace mjxsdk_impl {
        inline void* _Adjust_address_by_offset(void* const _Address, const size_t _Off) noexcept {
            // create a new address by moving the base address by the given offset
            return static_cast<void*>(static_cast<unsigned char*>(_Address) + _Off);
        }

        inline bool _Is_within_memory_block(const void* const _Base_begin,
            const void* const _Base_end, const void* const _Block_begin, const void* const _Block_end) noexcept {
            // check whether the memory block is within the base memory block
            return _Block_begin >= _Base_begin && _Block_end <= _Base_end;
        }

        constexpr size_t _Get_effective_alignment(const size_t _Align) noexcept {
            // returns the effective alignment for allocation and deallocation
            return _Align >= __STDCPP_DEFAULT_NEW_ALIGNMENT__ ? _Align : __STDCPP_DEFAULT_NEW_ALIGNMENT__;
        }
    } // namespace mjxsdk_impl
} // namespace mjx

#endif // _MJXSDK_MEMORY_IMPL_UTILS_HPP_