// smart_pointer.cpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#include <mjxsdk/memory/smart_pointer.hpp>

namespace mjx {
    reference_counter::reference_counter() noexcept : _Myrefs(0) {}

    reference_counter::reference_counter(const long _Refs) noexcept : _Myrefs(_Refs) {}

    reference_counter::~reference_counter() noexcept {}

    void reference_counter::increment() noexcept {
        ++_Myrefs;
    }

    long reference_counter::decrement() noexcept {
        return --_Myrefs;
    }

    long reference_counter::use_count() const noexcept {
        return _Myrefs.load(::std::memory_order_relaxed);
    }

    bool reference_counter::unique() const noexcept {
        return _Myrefs.load(::std::memory_order_relaxed) == 1;
    }
} // namespace mjx