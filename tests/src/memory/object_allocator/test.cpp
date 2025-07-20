// test.cpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#include <gtest/gtest.h>
#include <mjxsdk/memory/object_allocator.hpp>

namespace mjx {
    void _Capture_allocation_size(const size_t) noexcept;

    class tracking_allocator : public allocator {
    public:
        tracking_allocator() noexcept {}

        tracking_allocator(const tracking_allocator&) noexcept {}

        tracking_allocator(tracking_allocator&&) noexcept {}

        ~tracking_allocator() noexcept override {}

        tracking_allocator& operator=(const tracking_allocator&) noexcept {
            return *this;
        }

        tracking_allocator& operator=(tracking_allocator&&) noexcept {
            return *this;
        }

        pointer allocate(size_type _Count, size_type _Align = 0) override {
            if (_Align != 0) { // align _Count to the given alignment
                _Count = (_Count + _Align - 1) & ~(_Align - 1);
            }

            _Capture_allocation_size(_Count);
            return nullptr;
        }

        void deallocate(pointer, size_type _Count, size_type _Align = 0) noexcept override {
            if (_Align != 0) { // align _Count to the given alignment
                _Count = (_Count + _Align - 1) & ~(_Align - 1);
            }

            _Capture_allocation_size(_Count);
        }

        allocator_tag tag() const noexcept override {
            return allocator_tag{0xFF};
        }

        size_type max_size() const noexcept override {
            return static_cast<size_type>(-1);
        }

        bool is_equal(const allocator&) const noexcept override {
            return true;
        }
    };

    struct _Size_tracker {
        size_t _Size; // stores the size of the last allocation or deallocation
        tracking_allocator _Al; // an instance of the allocator used in the test

        _Size_tracker() noexcept : _Size(0), _Al() {
            ::mjx::set_global_allocator(_Al);
        }
    };

    _Size_tracker _Tracker;
    
    void _Capture_allocation_size(const size_t _Size) noexcept {
        _Tracker._Size = _Size;
    }

    struct alignas(32) _Aligned_type {
        uint8_t _Block[64] = {0};
    };

    static_assert(alignof(_Aligned_type) == 32, "_Aligned_type alignment mismatch");
    static_assert(sizeof(_Aligned_type) == 64, "_Aligned_type size mismatch");

    TEST(object_allocator, type_aligned_allocation_size) {
        constexpr size_t _Count         = 4;
        constexpr size_t _Expected_size = 256;
        object_allocator<_Aligned_type> _Al;
        _Al.allocate(_Count);
        EXPECT_EQ(_Tracker._Size, _Expected_size);
    }

    TEST(object_allocator, custom_aligned_allocation_size) {
        constexpr size_t _Count         = 7;
        constexpr size_t _Align         = 128;
        constexpr size_t _Expected_size = 512;
        object_allocator<_Aligned_type> _Al;
        _Al.allocate(_Count, _Align);
        EXPECT_EQ(_Tracker._Size, _Expected_size);
    }

    TEST(object_allocator, type_aligned_deallocation_size) {
        constexpr size_t _Count         = 15;
        constexpr size_t _Expected_size = 960;
        object_allocator<_Aligned_type> _Al;
        _Al.deallocate(nullptr, _Count);
        EXPECT_EQ(_Tracker._Size, _Expected_size);
    }

    TEST(object_allocator, custom_aligned_deallocation_size) {
        constexpr size_t _Count         = 21;
        constexpr size_t _Align         = 256;
        constexpr size_t _Expected_size = 1536;
        object_allocator<_Aligned_type> _Al;
        _Al.deallocate(nullptr, _Count, _Align);
        EXPECT_EQ(_Tracker._Size, _Expected_size);
    }
} // namespace mjx