// global_allocator.hpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef _MJXSDK_IMPL_GLOBAL_ALLOCATOR_HPP_
#define _MJXSDK_IMPL_GLOBAL_ALLOCATOR_HPP_
#include <atomic>
#include <mjxsdk/allocator.hpp>
#include <mjxsdk/native_allocator.hpp>

namespace mjx {
    namespace mjxsdk_impl {
        class _Global_allocator { // singleton class that stores the global allocator
        public:
            ~_Global_allocator() noexcept {}

            _Global_allocator(const _Global_allocator&)            = delete;
            _Global_allocator& operator=(const _Global_allocator&) = delete;

            static _Global_allocator& _Instance() noexcept {
                static _Global_allocator _Obj;
                return _Obj;
            }

            allocator& _Get() const noexcept {
                return *static_cast<allocator*>(_Mycur.load(::std::memory_order_relaxed));
            }

            void _Set(allocator& _New_al) noexcept {
                _Mycur.store(&_New_al, ::std::memory_order_relaxed);
            }

            void _Reset() noexcept {
                _Mycur.store(&_Mydef, ::std::memory_order_relaxed);
            }

        private:
            _Global_allocator() noexcept : _Mydef(), _Mycur(&_Mydef) {}

            native_allocator _Mydef; // the default allocator that has no state or associated resource
            ::std::atomic<void*> _Mycur; // pointer to the currently used allocator
        };

        inline native_allocator& _Get_internal_allocator() noexcept {
            // retrieve the singleton native_allocator used for all internal allocations
            static native_allocator _Al;
            return _Al;
        }
    } // namespace mjxsdk_impl
} // namespace mjx

#endif // _MJXSDK_IMPL_GLOBAL_ALLOCATOR_HPP_