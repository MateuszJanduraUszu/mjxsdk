// allocator.cpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#include <mjxsdk/allocator.hpp>

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
} // namespace mjx