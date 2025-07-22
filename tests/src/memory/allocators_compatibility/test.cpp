// test.cpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#include <gtest/gtest.h>
#include <mjxsdk/memory/allocator.hpp>
#include <mjxsdk/memory/system_allocator.hpp>

namespace mjx {
    class compatible_allocator : public allocator {
    public:
        compatible_allocator() noexcept {}
        
        compatible_allocator(const compatible_allocator&) noexcept {}

        compatible_allocator(compatible_allocator&&) noexcept {}

        ~compatible_allocator() noexcept override {}

        compatible_allocator& operator=(const compatible_allocator&) noexcept {
            return *this;
        }

        compatible_allocator& operator=(compatible_allocator&&) noexcept {
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

    class incompatible_allocator {
    public:
        using value_type      = void;
        using size_type       = size_t;
        using difference_type = ptrdiff_t;
        using pointer         = void*;

        incompatible_allocator() noexcept {}
        
        incompatible_allocator(const incompatible_allocator&) noexcept {}

        incompatible_allocator(incompatible_allocator&&) noexcept {}

        ~incompatible_allocator() noexcept {}

        incompatible_allocator& operator=(const incompatible_allocator&) noexcept {
            return *this;
        }

        incompatible_allocator& operator=(incompatible_allocator&&) noexcept {
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
        EXPECT_TRUE(is_compatible_allocator_v<compatible_allocator>);
        EXPECT_FALSE(is_compatible_allocator_v<incompatible_allocator>);
    }
} // namespace mjx