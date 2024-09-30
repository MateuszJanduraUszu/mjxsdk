// utils.hpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef _MJXSDK_IMPL_UTILS_HPP_
#define _MJXSDK_IMPL_UTILS_HPP_
#include <type_traits>

namespace mjx {
    namespace mjxsdk_impl {
        template <class _Integer>
        constexpr bool _Is_pow_of_2(const _Integer _Value) noexcept {
            // check if the given value is a power of 2 (only one bit is set)
            static_assert(::std::is_unsigned_v<_Integer>, "_Integer must be unsigned");
            return _Value > 0 && (_Value & (_Value - 1)) == 0;
        }

        template <class _Integer>
        constexpr _Integer _Align_value(const _Integer _Value, const _Integer _Align) noexcept {
            // align the given value to the nearest multiple of the given alignment
            static_assert(::std::is_unsigned_v<_Integer>, "_Integer must be unsigned");
            return (_Value + _Align - 1) & ~(_Align - 1);
        }
    } // namespace mjxsdk_impl
} // namespace mjx

#endif // _MJXSDK_IMPL_UTILS_HPP_