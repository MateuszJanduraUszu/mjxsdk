// test.cpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#include <gtest/gtest.h>
#include <mjxsdk/allocator.hpp>

namespace mjx {
    class custom_allocator : public allocator {
    public:
        custom_allocator() noexcept {}

        custom_allocator(const custom_allocator&) noexcept {}

        custom_allocator(custom_allocator&&) noexcept {}

        ~custom_allocator() noexcept override {}

        custom_allocator& operator=(const custom_allocator&) noexcept {
            return *this;
        }

        custom_allocator& operator=(custom_allocator&&) noexcept {
            return *this;
        }

        pointer allocate(size_type, const size_type = 0) override {
            if (_Myctr) {
                ++*_Myctr;
            }

            return nullptr;
        }

        void deallocate(pointer, size_type, const size_type = 0) noexcept override {
            if (_Myctr) {
                ++*_Myctr;
            }
        }

        size_type max_size() const noexcept override {
            if (_Myctr) {
                ++*_Myctr;
            }

            return 0;
        }

        bool is_equal(const allocator&) const noexcept override {
            if (_Myctr) {
                ++*_Myctr;
            }

            return true;
        }

        void set_counter(size_t& _Counter) noexcept {
            _Myctr = &_Counter;
        }

    private:
        size_t* _Myctr = nullptr;
    };

    TEST(global_allocator, use_custom_allocator) {
        constexpr size_t _Expected_counter = 4;
        size_t _Counter                    = 0;
        custom_allocator _Al;
        _Al.set_counter(_Counter);
        ::mjx::set_global_allocator(_Al);

        // call all global allocator methods, each call should increment the counter
        allocator& _Global_al = ::mjx::get_global_allocator();
        _Global_al.allocate(0);
        _Global_al.deallocate(nullptr, 0);
        _Global_al.max_size();
        _Global_al.is_equal(_Al);
        EXPECT_EQ(_Counter, _Expected_counter);
    }
} // namespace mjx