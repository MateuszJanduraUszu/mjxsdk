// allocator.hpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef _MJXSDK_ALLOCATOR_HPP_
#define _MJXSDK_ALLOCATOR_HPP_
#include <cstddef>
#include <mjxsdk/core.hpp>
#include <mjxsdk/export.hpp>

namespace mjx {
    class _MJX_NOVTABLE _MJXSDK_EXPORT allocator { // base class for all allocators
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

        // allocates uninitialized storage
        virtual pointer allocate(const size_type _Count) = 0;

        // allocates uninitialized storage with the specifed alignment
        virtual pointer allocate_aligned(const size_type _Count, const size_type _Align) = 0;

        // deallocates storage
        virtual void deallocate(pointer _Ptr, const size_type _Count) noexcept = 0;

        // deallocates aligned storage
        virtual void deallocate_aligned(
            pointer _Ptr, const size_type _Count, const size_type _Align) noexcept = 0;

        // returns the largest supported allocation size
        virtual size_type max_size() const noexcept = 0;

        // compares for equality with another allocator
        virtual bool is_equal(const allocator& _Other) const noexcept = 0;
    };

    _MJXSDK_EXPORT bool operator==(const allocator& _Left, const allocator& _Right) noexcept;
} // namespace mjx

#endif // _MJXSDK_ALLOCATOR_HPP_