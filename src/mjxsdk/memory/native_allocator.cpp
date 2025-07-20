// native_allocator.cpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#include <mjxsdk/core/impl/assert.hpp>
#include <mjxsdk/core/impl/utils.hpp>
#include <mjxsdk/memory/exception.hpp>
#include <mjxsdk/memory/native_allocator.hpp>
#include <new>
#include <utility>
#ifdef _DEBUG
#include <mjxsdk/memory/impl/debug_block.hpp>
#endif // _DEBUG

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

#ifdef _DEBUG
    native_allocator::pointer
        native_allocator::_Allocate_debug(const size_type _Size, const size_type _Align) {
        _INTERNAL_ASSERT(mjxsdk_impl::_Is_zero_or_pow_of_2(_Align), "alignment must be a power of 2");
        void* const _Ptr = mjxsdk_impl::_Allocate_debug_block(_Size, _Align, allocator_tag::native,
            [](const size_type _Size, const size_type _Align) noexcept {
                return ::operator new(_Size, ::std::align_val_t{_Align}, ::std::nothrow);
            }
        );
        if (!_Ptr) { // allocation failed, raise an exception
            allocation_failure::raise();
        }
        
        return _Ptr;
    }

    void native_allocator::_Deallocate_debug(
        pointer _Ptr, const size_type _Size, const size_type _Align) noexcept {
        _INTERNAL_ASSERT(mjxsdk_impl::_Is_zero_or_pow_of_2(_Align), "alignment must be a power of 2");
        mjxsdk_impl::_Deallocate_debug_block(_Ptr, _Size, _Align, allocator_tag::native,
            [](pointer _Ptr, const size_type _Size, const size_type _Align) noexcept {
                ::operator delete(_Ptr, _Size, ::std::align_val_t{_Align});
            }
        );
    }
#endif // _DEBUG

    native_allocator::pointer native_allocator::allocate(size_type _Size, size_type _Align) {
        if (_Size == 0) { // no allocation, do nothing
            return nullptr;
        }

#ifdef _DEBUG
        return _Allocate_debug(_Size, _Align);
#else // ^^^ _DEBUG ^^^ / vvv NDEBUG vvv
        void* _Ptr = nullptr;
        if (_Align != 0) { // use the given alignment
            _Ptr = ::operator new(_Size, ::std::align_val_t{_Align}, ::std::nothrow);
        } else { // use the default alignment
            _Ptr = ::operator new(_Size, ::std::nothrow);
        }

        if (!_Ptr) { // allocation failed, raise an exception
            allocation_failure::raise();
        }

        return _Ptr;
#endif // _DEBUG
    }

    void native_allocator::deallocate(pointer _Ptr, size_type _Size, size_type _Align) noexcept {
        if (!_Ptr || _Size == 0) { // invalid block, break
            return;
        }

#ifdef _DEBUG
        _Deallocate_debug(_Ptr, _Size, _Align);
#else // ^^^ _DEBUG ^^^ / vvv NDEBUG vvv
        if (_Align != 0) { // use the given alignment
            ::operator delete(_Ptr, _Size, ::std::align_val_t{_Align});
        } else { // use the default alignment
            ::operator delete(_Ptr, _Size);
        }
#endif // _DEBUG
    }

    allocator_tag native_allocator::tag() const noexcept {
        return allocator_tag::native;
    }

    native_allocator::size_type native_allocator::max_size() const noexcept {
        return static_cast<size_type>(-1);
    }

    bool native_allocator::is_equal(const allocator& _Other) const noexcept {
        // cast to a pointer to avoid a bad_cast exception
        return dynamic_cast<const native_allocator*>(::std::addressof(_Other)) != nullptr;
    }
} // namespace mjx