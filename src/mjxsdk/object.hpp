// object.hpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef _MJXSDK_OBJECT_HPP_
#define _MJXSDK_OBJECT_HPP_
#include <mjxsdk/allocator.hpp>
#include <new>
#include <type_traits>
#include <utility>

namespace mjx {
    template <class _Ty, class... _Types>
    inline _Ty* construct_object(_Ty* const _Location, _Types&&... _Args)
        noexcept(::std::is_nothrow_constructible_v<_Ty, _Types...>) {
        // construct an object in-place at the given memory location
        return ::new (static_cast<void*>(_Location)) _Ty(::std::forward<_Types>(_Args)...);
    }

    template <class _Ty>
    inline void destroy_object(_Ty* const _Obj) noexcept(::std::is_nothrow_destructible_v<_Ty>) {
        // destroy the object (if it has a non-trivial destructor)
        if constexpr (!::std::is_trivially_destructible_v<_Ty>) {            
            _Obj->~_Ty(); // non-trivial destructor, call it
        }
    }

    template <class _Ty, compatible_allocator _Alloc>
    inline _Ty* allocate_object_using_allocator(_Alloc& _Al) {
        // allocate memory for an object using the given allocator
        return static_cast<_Ty*>(_Al.allocate(sizeof(_Ty)));
    }
    
    template <class _Ty, compatible_allocator _Alloc>
    inline _Ty* allocate_object_array_using_allocator(const size_t _Count, _Alloc& _Al) {
        // allocate memory for an array of objects using the given allocator
        return static_cast<_Ty*>(_Al.allocate(_Count * sizeof(_Ty)));
    }

    template <class _Ty, compatible_allocator _Alloc>
    inline void deallocate_object_using_allocator(_Ty* const _Obj, _Alloc& _Al) noexcept {
        // deallocate the object's memory using the given allocator
        _Al.deallocate(_Obj, sizeof(_Ty));
    }

    template <class _Ty, compatible_allocator _Alloc>
    inline void deallocate_object_array_using_allocator(
        _Ty* const _Array, const size_t _Count, _Alloc& _Al) noexcept {
        // deallocate the array's memory using the given allocator
        _Al.deallocate(_Array, _Count * sizeof(_Ty));
    }

    template <class _Ty, compatible_allocator _Alloc, class... _Types>
    inline _Ty* create_object_using_allocator(_Alloc& _Al, _Types&&... _Args) {
        // allocate memory for an object using the given allocator, then construct the object in-place
        return ::mjx::construct_object(
            ::mjx::allocate_object_using_allocator<_Ty>(_Al), ::std::forward<_Types>(_Args)...);
    }

    template <class _Ty, compatible_allocator _Alloc>
    inline _Ty* create_object_array_using_allocator(const size_t _Count, _Alloc& _Al) {
        // allocate memory for an array of objects using the given allocator,
        // then construct the objects in-place
        _Ty* const _Ptr = ::mjx::allocate_object_array_using_allocator<_Ty>(_Count, _Al);
        for (size_t _Off = 0; _Off < _Count; ++_Off) { // construct each object separately
            ::mjx::construct_object(_Ptr + _Off);
        }

        return _Ptr;
    }

    template <class _Ty, compatible_allocator _Alloc>
    inline void delete_object_using_allocator(_Ty* const _Obj, _Alloc& _Al)
        noexcept(::std::is_nothrow_destructible_v<_Ty>) {
        // destroy the object (if it has a non-trivial destructor)
        // and deallocate its memory using the given allocator
        if (_Obj) {
            ::mjx::destroy_object(_Obj);
            ::mjx::deallocate_object_using_allocator(_Obj, _Al);
        }
    }

    template <class _Ty, compatible_allocator _Alloc>
    inline void delete_object_array_using_allocator(_Ty* const _Array, const size_t _Count, _Alloc& _Al)
        noexcept(::std::is_nothrow_destructible_v<_Ty>) {
        // destroy each object in the array (if they have non-trivial destructors)
        // and deallocate the array's memory using the given allocator
        if (_Array && _Count > 0) {
            if constexpr (!::std::is_trivially_destructible_v<_Ty>) {
                for (size_t _Idx = 0; _Idx < _Count; ++_Idx) { // destroy each object separately
                    ::mjx::destroy_object(::std::addressof(_Array[_Idx]));
                }
            }

            ::mjx::deallocate_object_array_using_allocator(_Array, _Count, _Al);
        }
    }

    template <class _Ty>
    inline _Ty* allocate_object() {
        // allocate memory for an object using the global allocator
        return ::mjx::allocate_object_using_allocator<_Ty>(::mjx::get_global_allocator());
    }

    template <class _Ty>
    inline _Ty* allocate_object_array(const size_t _Count) {
        // allocate memory for an array of objects using the global allocator
        return ::mjx::allocate_object_array_using_allocator<_Ty>(_Count, ::mjx::get_global_allocator());
    }

    template <class _Ty>
    inline void deallocate_object(_Ty* const _Obj) noexcept {
        // deallocate the object's memory using the global allocator
        ::mjx::deallocate_object_using_allocator(_Obj, ::mjx::get_global_allocator());
    }

    template <class _Ty>
    inline void deallocate_object_array(_Ty* const _Array, const size_t _Count) noexcept {
        // deallocate the array's memory using the global allocator
        ::mjx::deallocate_object_array_using_allocator(_Array, _Count, ::mjx::get_global_allocator());
    }

    template <class _Ty, class... _Types>
    inline _Ty* create_object(_Types&&... _Args) {
        // allocate memory for an object using the global allocator, then construct the object in-place
        return ::mjx::create_object_using_allocator<_Ty>(
            ::mjx::get_global_allocator(), ::std::forward<_Types>(_Args)...);
    }

    template <class _Ty>
    inline _Ty* create_object_array(const size_t _Count) {
        // allocate memory for an array of objects using the given allocator,
        // then construct the objects in-place
        return ::mjx::create_object_array_using_allocator<_Ty>(_Count, ::mjx::get_global_allocator());
    }

    template <class _Ty>
    inline void delete_object(_Ty* const _Obj) noexcept(::std::is_nothrow_destructible_v<_Ty>) {
        // destroy the object (if it has a non-trivial destructor)
        // and deallocate its memory using the global allocator
        ::mjx::delete_object_using_allocator(_Obj, ::mjx::get_global_allocator());
    }

    template <class _Ty>
    inline void delete_object_array(_Ty* const _Array, const size_t _Count)
        noexcept(::std::is_nothrow_destructible_v<_Ty>) {
        // destroy each object in the array (if they have non-trivial destructors)
        // and deallocate the array's memory using the global allocator
        ::mjx::delete_object_array_using_allocator(_Array, _Count, ::mjx::get_global_allocator());
    }
} // namespace mjx

#endif // _MJXSDK_OBJECT_HPP_