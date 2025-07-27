// test.cpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#include <gtest/gtest.h>
#include <mjxsdk/memory/allocator.hpp>
#include <mjxsdk/memory/system_allocator.hpp>

namespace mjx {
    class comp_allocator : public allocator { // allocator that is compatible with the built-in allocators
    public:
        comp_allocator() noexcept {}
        
        comp_allocator(const comp_allocator&) noexcept {}

        comp_allocator(comp_allocator&&) noexcept {}

        ~comp_allocator() noexcept override {}

        comp_allocator& operator=(const comp_allocator&) noexcept {
            return *this;
        }

        comp_allocator& operator=(comp_allocator&&) noexcept {
            return *this;
        }

        pointer allocate(size_type, size_type = 0) override {
            return nullptr;
        }

        void deallocate(pointer, size_type, size_type = 0) noexcept override {}

        allocator_tag tag() const noexcept override {
            return allocator_tag{0xFF};
        }

        size_type max_size() const noexcept override {
            return 0;
        }

        bool is_equal(const allocator&) const noexcept override {
            return true;
        }
    };

    class incomp_allocator { // allocator that is incompatible with the built-in allocators
    public:
        using value_type      = void;
        using size_type       = size_t;
        using difference_type = ptrdiff_t;
        using pointer         = void*;
        using const_pointer   = const void*;

        incomp_allocator() noexcept {}
        
        incomp_allocator(const incomp_allocator&) noexcept {}

        incomp_allocator(incomp_allocator&&) noexcept {}

        ~incomp_allocator() noexcept {}

        incomp_allocator& operator=(const incomp_allocator&) noexcept {
            return *this;
        }

        incomp_allocator& operator=(incomp_allocator&&) noexcept {
            return *this;
        }

        pointer allocate(size_type, size_type = 0) {
            return nullptr;
        }

        void deallocate(pointer, size_type, size_type = 0) noexcept {}

        allocator_tag tag() const noexcept {
            return allocator_tag{0xFF};
        }

        size_type max_size() const noexcept {
            return 0;
        }

        bool is_equal(const allocator&) const noexcept {
            return true;
        }
    };

    TEST(allocators_compatibility, builtin_allocators) {
        EXPECT_TRUE(is_compatible_allocator_v<system_allocator>);
    }

    TEST(allocators_compatibility, custom_allocators) {
        EXPECT_TRUE(is_compatible_allocator_v<comp_allocator>);
        EXPECT_FALSE(is_compatible_allocator_v<incomp_allocator>);
    }
} // namespace mjx