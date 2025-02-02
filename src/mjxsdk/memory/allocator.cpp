// allocator.cpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#include <mjxsdk/memory/allocator.hpp>
#include <mjxsdk/memory/impl/global_allocator.hpp>

namespace mjx {
    allocator::allocator() noexcept {}

    allocator::allocator(const allocator&) noexcept {}

    allocator::allocator(allocator&&) noexcept {}

    allocator::~allocator() noexcept {}

    allocator& allocator::operator=(const allocator&) noexcept {
        return *this;
    }

    allocator& allocator::operator=(allocator&&) noexcept {
        return *this;
    }

    bool operator==(const allocator& _Left, const allocator& _Right) noexcept {
        return _Left.is_equal(_Right);
    }

    allocator& get_global_allocator() noexcept {
        return mjxsdk_impl::_Global_allocator::_Instance()._Get();
    }

    void set_global_allocator(allocator& _New_al) noexcept {
        mjxsdk_impl::_Global_allocator::_Instance()._Set(_New_al);
    }

    void reset_global_allocator() noexcept {
        mjxsdk_impl::_Global_allocator::_Instance()._Reset();
    }
} // namespace mjx