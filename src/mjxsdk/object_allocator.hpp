// object_allocator.hpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef _MJXSDK_OBJECT_ALLOCATOR_HPP_
#define _MJXSDK_OBJECT_ALLOCATOR_HPP_
#include <mjxsdk/allocator.hpp>
#include <type_traits>

namespace mjx {
    template <class _Ty>
    class object_allocator { // type-specific wrapper around the global allocator
    public:
        static_assert(!::std::is_same_v<_Ty, void> && !::std::is_const_v<_Ty>
            && !::std::is_reference_v<_Ty> && !::std::is_function_v<_Ty>
            && !::std::is_volatile_v<_Ty>, "invalid value type");

        using value_type      = _Ty;
        using size_type       = allocator::size_type;
        using difference_type = allocator::difference_type;
        using pointer         = _Ty*;
        using const_pointer   = const _Ty*;
        using reference       = _Ty&;
        using const_reference = const _Ty&;

        template <class _Other>
        struct rebind {
            using other = object_allocator<_Other>;
        };

        object_allocator() noexcept                        = default;
        object_allocator(const object_allocator&) noexcept = default;
        object_allocator(object_allocator&&) noexcept      = default;
        ~object_allocator() noexcept                       = default;
    
        template <class _Other>
        object_allocator(const object_allocator<_Other>&) noexcept {}

        object_allocator& operator=(const object_allocator&) noexcept = default;
        object_allocator& operator=(object_allocator&&) noexcept      = default;

        template <class _Other>
        object_allocator& operator=(const object_allocator<_Other>&) noexcept {
            return *this;
        }

        pointer allocate(size_type _Count, const size_type _Align = 0) {
            return static_cast<pointer>(::mjx::get_global_allocator().allocate(_Count * sizeof(_Ty), _Align));
        }

        void deallocate(pointer _Ptr, size_type _Count, const size_type _Align = 0) noexcept {
            ::mjx::get_global_allocator().deallocate(_Ptr, _Count * sizeof(_Ty), _Align);
        }

        size_type max_size() const noexcept {
            return ::mjx::get_global_allocator().max_size() / sizeof(_Ty);
        }

        bool is_equal(const object_allocator&) const noexcept {
            return true; // always equal
        }
    };

    template <class _Ty1, class _Ty2>
    constexpr bool operator==(const object_allocator<_Ty1>&, const object_allocator<_Ty2>&) noexcept {
        return true; // always equal
    }
} // namespace mjx

#endif // _MJXSDK_OBJECT_ALLOCATOR_HPP_