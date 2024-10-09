// object.hpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef _MJXSDK_OBJECT_HPP_
#define _MJXSDK_OBJECT_HPP_
#include <mjxsdk/object_allocator.hpp>
#include <new>
#include <type_traits>
#include <utility>

namespace mjx {
    template <class _Ty, class... _Types>
    inline _Ty* create_object(_Types&&... _Args) {
        // allocate memory for an object and construct it in-place
        object_allocator<_Ty> _Al;
        return ::new (static_cast<void*>(_Al.allocate(1))) _Ty(::std::forward<_Types>(_Args)...);
    }

    template <class _Ty>
    inline void delete_object(_Ty* const _Obj) noexcept(::std::is_nothrow_destructible_v<_Ty>) {
        // destroy the object (if it has a non-trivial destructor) and deallocate its memory
        if (_Obj) {
            if constexpr (!::std::is_trivially_destructible_v<_Ty>) {
                _Obj->~_Ty(); // non-trivial destructor, call it
            }

            object_allocator<_Ty> _Al;
            _Al.deallocate(_Obj, 1);
        }
    }

    template <class _Ty>
    inline _Ty* allocate_object_array(const size_t _Count) {
        // allocate memory for an array of objects
        object_allocator<_Ty> _Al;
        return _Al.allocate(_Count);
    }

    template <class _Ty>
    inline void delete_object_array(_Ty* const _Array, const size_t _Count)
        noexcept(::std::is_nothrow_destructible_v<_Ty>) {
        // destroy each object in the array (if they have non-trivial destructors)
        // and deallocate the array's memory
        if (_Array && _Count > 0) {
            if constexpr (!::std::is_trivially_destructible_v<_Ty>) {
                for (size_t _Idx = 0; _Idx < _Count; ++_Idx) {
                    _Array[_Idx].~_Ty(); // non-trivial destructor, call it
                }
            }

            object_allocator<_Ty> _Al;
            _Al.deallocate(_Array, _Count);
        }
    }

    template <class _Ty, compatible_allocator _Alloc, class... _Types>
    inline _Ty* create_object_using_allocator(_Alloc& _Al, _Types&&... _Args) {
        // allocate memory for an object using the given allocator, then construct the object in-place
        return ::new (static_cast<void*>(_Al.allocate(sizeof(_Ty)))) _Ty(::std::forward<_Types>(_Args)...);
    }

    template <class _Ty, compatible_allocator _Alloc>
    inline void delete_object_using_allocator(_Ty* const _Obj, _Alloc& _Al)
        noexcept(::std::is_nothrow_destructible_v<_Ty>) {
        // destroy the object (if it has a non-trivial destructor)
        // and deallocate its memory using the given allocator
        if (_Obj) {
            if constexpr (!::std::is_trivially_destructible_v<_Ty>) {
                _Obj->~_Ty(); // non-trivial destructor, call it
            }

            _Al.deallocate(_Obj, sizeof(_Ty));
        }
    }

    template <class _Ty, compatible_allocator _Alloc>
    inline _Ty* allocate_object_array_using_allocator(const size_t _Count, _Alloc& _Al) {
        // allocate memory for an array of objects using the given allocator
        return static_cast<_Ty*>(_Al.allocate(_Count * sizeof(_Ty)));
    }

    template <class _Ty, compatible_allocator _Alloc>
    inline void delete_object_array_using_allocator(_Ty* const _Array, const size_t _Count, _Alloc& _Al)
        noexcept(::std::is_nothrow_destructible_v<_Ty>) {
        // destroy each object in the array (if they have non-trivial destructors) and deallocate
        // the array's memory using the given allocator
        if (_Array && _Count > 0) {
            if constexpr (!::std::is_trivially_destructible_v<_Ty>) {
                for (size_t _Idx = 0; _Idx < _Count; ++_Idx) {
                    _Array[_Idx].~_Ty(); // non-trivial destructor, call it
                }
            }

            _Al.deallocate(_Array, _Count * sizeof(_Ty));
        }
    }
} // namespace mjx

#endif // _MJXSDK_OBJECT_HPP_