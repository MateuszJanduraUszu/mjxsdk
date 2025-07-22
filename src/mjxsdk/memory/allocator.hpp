// allocator.hpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef _MJXSDK_MEMORY_ALLOCATOR_HPP_
#define _MJXSDK_MEMORY_ALLOCATOR_HPP_
#include <cstddef>
#include <mjxsdk/core/export.hpp>
#include <mjxsdk/core/macros.hpp>
#include <type_traits>

namespace mjx {
    enum class allocator_tag : unsigned char {
        unknown = 0,
        system  = 1
    };

    class _MJXSDK_EXPORT _MJX_NOVTABLE allocator { // base class for all allocators
    public:
        using value_type      = void;
        using size_type       = size_t;
        using difference_type = ptrdiff_t;
        using pointer         = void*;

        allocator() noexcept;
        allocator(const allocator& _Other) noexcept;
        allocator(allocator&& _Other) noexcept;
        virtual ~allocator() noexcept;

        allocator& operator=(const allocator& _Other) noexcept;
        allocator& operator=(allocator&& _Other) noexcept;

        // allocates uninitialized storage with optional alignment
        virtual pointer allocate(size_type _Size, size_type _Align = 0) = 0;

        // deallocates storage with optional alignment
        virtual void deallocate(pointer _Ptr, size_type _Size, size_type _Align = 0) noexcept = 0;

        // returns the tag that identifies the allocator type
        virtual allocator_tag tag() const noexcept = 0;

        // returns the largest supported allocation size
        virtual size_type max_size() const noexcept = 0;

        // compares for equality with another allocator
        virtual bool is_equal(const allocator& _Other) const noexcept = 0;
    };

    _MJXSDK_EXPORT bool operator==(const allocator& _Left, const allocator& _Right) noexcept;

    template <class _Alloc>
    inline constexpr bool is_compatible_allocator_v = ::std::is_base_of_v<allocator, _Alloc>;

    template <class _Alloc>
    struct is_compatible_allocator : ::std::bool_constant<is_compatible_allocator_v<_Alloc>> {};

    template <class _Alloc>
    concept compatible_allocator = is_compatible_allocator_v<_Alloc>;

    _MJXSDK_EXPORT allocator& get_global_allocator() noexcept;
    _MJXSDK_EXPORT void set_global_allocator(allocator& _New_al) noexcept;
    _MJXSDK_EXPORT void reset_global_allocator() noexcept;
} // namespace mjx

#endif // _MJXSDK_MEMORY_ALLOCATOR_HPP_