// memory_resource.hpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef _MJXSDK_MEMORY_MEMORY_RESOURCE_HPP_
#define _MJXSDK_MEMORY_MEMORY_RESOURCE_HPP_
#include <mjxsdk/core/export.hpp>
#include <mjxsdk/memory/allocator.hpp>

namespace mjx {
    class _MJXSDK_EXPORT memory_resource { // memory block that serves as a resource for allocators
    public:
        using size_type       = allocator::size_type;
        using difference_type = allocator::difference_type;
        using pointer         = allocator::pointer;
        using const_pointer   = const pointer;
    
        memory_resource() noexcept;
        memory_resource(const memory_resource& _Other);
        memory_resource(memory_resource&& _Other) noexcept;
        ~memory_resource() noexcept;

        explicit memory_resource(const size_type _Size);

        memory_resource& operator=(const memory_resource& _Other);
        memory_resource& operator=(memory_resource&& _Other) noexcept;
        
        // checks whether the resource is empty
        bool empty() const noexcept;

        // returns the stored data
        pointer data() noexcept;
        const_pointer data() const noexcept;

        // returns the size of the resource
        size_type size() const noexcept;
        
        // checks whether the specified memory block comes from this resource
        bool contains(const_pointer _Block, const size_type _Size) const noexcept;

        // swaps two resources
        void swap(memory_resource& _Other) noexcept;

        struct release_result {
            pointer ptr;
            size_type size;
        };

        // releases the resource
        release_result release() noexcept;

        // destroys the resource
        void destroy() noexcept;

    private:
        // copies other resource
        void _Copy_resource(const_pointer _Ptr, const size_type _Size);

        // moves other resource
        void _Move_resource(pointer& _Ptr, size_type& _Size) noexcept;

        pointer _Myptr;
        size_type _Mysize;
    };

    _MJXSDK_EXPORT bool operator==(const memory_resource& _Left, const memory_resource& _Right) noexcept;
} // namespace mjx

#endif // _MJXSDK_MEMORY_MEMORY_RESOURCE_HPP_