// native_allocator.cpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#include <mjxsdk/core/impl/assert.hpp>
#include <mjxsdk/core/impl/utils.hpp>
#include <mjxsdk/memory/exception.hpp>
#include <mjxsdk/memory/native_allocator.hpp>
#include <new>
#include <utility>

namespace mjx {
    native_allocator::native_allocator() noexcept {}

    native_allocator::native_allocator(const native_allocator&) noexcept {}

    native_allocator::native_allocator(native_allocator&&) noexcept {}

    native_allocator::~native_allocator() noexcept {}

    native_allocator& native_allocator::operator=(const native_allocator&) noexcept {
        return *this;
    }

    native_allocator& native_allocator::operator=(native_allocator&&) noexcept {
        return *this;
    }

    native_allocator::pointer native_allocator::allocate(size_type _Count, size_type _Align) {
        if (_Count == 0) { // no allocation, do nothing
            return nullptr;
        }

        void* _Ptr = nullptr;
        if (_Align != 0) { // use the given alignment
#ifdef _DEBUG
            _INTERNAL_ASSERT(mjxsdk_impl::_Is_pow_of_2(_Align), "alignment must be a power of 2");
#endif // _DEBUG
            _Ptr = ::operator new(_Count, static_cast<::std::align_val_t>(_Align), ::std::nothrow);
        } else { // use the default alignment
            _Ptr = ::operator new(_Count, ::std::nothrow);
        }

        if (!_Ptr) { // allocation failed, raise an exception
            allocation_failure::raise();
        }

        return _Ptr;
    }

    void native_allocator::deallocate(pointer _Ptr, size_type _Count, size_type _Align) noexcept {
        if (!_Ptr || _Count == 0) { // invalid block, break
            return;
        }

        if (_Align != 0) { // use the given alignment
#ifdef _DEBUG
            _INTERNAL_ASSERT(mjxsdk_impl::_Is_pow_of_2(_Align), "alignment must be a power of 2");
#endif // _DEBUG
            ::operator delete(_Ptr, _Count, static_cast<::std::align_val_t>(_Align));
        } else { // use the default alignment
            ::operator delete(_Ptr, _Count);
        }
    }

    native_allocator::size_type native_allocator::max_size() const noexcept {
        return static_cast<size_type>(-1);
    }

    bool native_allocator::is_equal(const allocator& _Other) const noexcept {
        // cast to a pointer to avoid a bad_cast exception
        return dynamic_cast<const native_allocator*>(::std::addressof(_Other)) != nullptr;
    }
} // namespace mjx