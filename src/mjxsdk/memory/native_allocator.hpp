// native_allocator.hpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef _MJXSDK_MEMORY_NATIVE_ALLOCATOR_HPP_
#define _MJXSDK_MEMORY_NATIVE_ALLOCATOR_HPP_
#include <mjxsdk/core/export.hpp>
#include <mjxsdk/memory/allocator.hpp>

namespace mjx {
    class _MJXSDK_EXPORT native_allocator : public allocator { // stateless memory allocator
    public:
        using value_type      = allocator::value_type;
        using size_type       = allocator::size_type;
        using difference_type = allocator::difference_type;
        using pointer         = allocator::pointer;

        native_allocator() noexcept;
        native_allocator(const native_allocator& _Other) noexcept;
        native_allocator(native_allocator&& _Other) noexcept;
        ~native_allocator() noexcept override;

        native_allocator& operator=(const native_allocator& _Other) noexcept;
        native_allocator& operator=(native_allocator&& _Other) noexcept;

        // allocates uninitialized storage with optional alignment
        pointer allocate(size_type _Size, size_type _Align = 0) override;

        // deallocates storage with optional alignment
        void deallocate(pointer _Ptr, size_type _Size, size_type _Align = 0) noexcept override;

        // returns the tag that identifies the allocator type
        allocator_tag tag() const noexcept override;

        // returns the largest supported allocation size
        size_type max_size() const noexcept override;

        // compares for equality with another allocator
        bool is_equal(const allocator& _Other) const noexcept override;

    private:
#ifdef _DEBUG
        // allocates unintialized storage with optional alignment for debug mode
        static pointer _Allocate_debug(const size_type _Size, const size_type _Align);

        // deallocates storage with optional alignment for debug mode
        static void _Deallocate_debug(
            pointer _Ptr, const size_type _Size, const size_type _Align) noexcept;
#endif // _DEBUG
    };
} // namespace mjx

#endif // _MJXSDK_MEMORY_NATIVE_ALLOCATOR_HPP_