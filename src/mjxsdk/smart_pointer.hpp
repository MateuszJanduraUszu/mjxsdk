// smart_pointer.hpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef _MJXSDK_SMART_POINTER_HPP_
#define _MJXSDK_SMART_POINTER_HPP_
#include <atomic>
#include <mjxsdk/exception.hpp>
#include <mjxsdk/object.hpp>
#include <type_traits>
#include <utility>

namespace mjx {
    template <class _Ty>
    class unique_ptr { // smart pointer with unique object ownership semantics
    public:
        using element_type = _Ty;
        using pointer      = _Ty*;

        unique_ptr() noexcept : _Myptr(nullptr) {}

        unique_ptr(::std::nullptr_t) noexcept : _Myptr(nullptr) {}
        
        explicit unique_ptr(pointer _Ptr) noexcept : _Myptr(_Ptr) {}

        unique_ptr(unique_ptr&& _Other) noexcept : _Myptr(_Other.release()) {}

        ~unique_ptr() noexcept {
            ::mjx::delete_object(_Myptr);
        }

        unique_ptr& operator=(unique_ptr&& _Other) noexcept {
            reset(_Other.release());
            return *this;
        }

        unique_ptr& operator=(::std::nullptr_t) noexcept {
            reset();
            return *this;
        }

        unique_ptr(const unique_ptr&)            = delete;
        unique_ptr& operator=(const unique_ptr&) = delete;

        explicit operator bool() const noexcept {
            return _Myptr != nullptr;
        }

        element_type& operator*() const noexcept {
            // the behavior is undefined if the stored pointer is null
            return *_Myptr;
        }

        pointer operator->() const noexcept {
            return _Myptr;
        }

        pointer get() const noexcept {
            return _Myptr;
        }

        pointer release() noexcept {
            pointer _Old_ptr = _Myptr;
            _Myptr           = nullptr;
            return _Old_ptr;
        }

        void reset(pointer _New_ptr = nullptr) noexcept {
            if (_Myptr) { // delete the currently managed object
                ::mjx::delete_object(_Myptr);
            }

            _Myptr = _New_ptr;
        }

        void swap(unique_ptr& _Other) noexcept {
            ::std::swap(_Myptr, _Other._Myptr);
        }

    private:
        pointer _Myptr;
    };

    template <class _Ty>
    inline bool operator==(const unique_ptr<_Ty>& _Left, const unique_ptr<_Ty>& _Right) noexcept {
        return _Left.get() == _Right.get();
    }

    template <class _Ty>
    inline bool operator==(const unique_ptr<_Ty>& _Left, ::std::nullptr_t) noexcept {
        return _Left.get() == nullptr;
    }

    template <class _Ty>
    inline ::std::strong_ordering operator<=>(const unique_ptr<_Ty>& _Left, const unique_ptr<_Ty>& _Right) noexcept {
        return _Left.get() <=> _Right.get();
    }

    template <class _Ty, class... _Types>
    inline unique_ptr<_Ty> make_unique(_Types&&... _Args) {
        // create a unique pointer that manages a newly created object
        return unique_ptr<_Ty>(::mjx::create_object<_Ty>(::std::forward<_Types>(_Args)...));
    }

    template <class _Ty>
    inline unique_ptr<_Ty> make_unique(const _Ty& _Value) {
        // create a unique pointer that manages a newly copy-constructed object
        return unique_ptr<_Ty>(::mjx::create_object<_Ty>(_Value));
    }

    template <class _Ty>
    inline unique_ptr<_Ty> make_unique_for_overwrite() {
        // create a unique pointer that manages a newly default-initialized object
        return unique_ptr<_Ty>(::mjx::create_object<_Ty>());
    }

    template <class _Ty>
    class unique_array { // smart pointer with unique object ownership semantics for arrays
    public:
        using element_type = _Ty;
        using pointer      = _Ty*;

        unique_array() noexcept : _Myptr(nullptr), _Mysize(0) {}

        unique_array(::std::nullptr_t, const size_t) noexcept : _Myptr(nullptr), _Mysize(0) {}

        explicit unique_array(pointer _Ptr, const size_t _Size) noexcept : _Myptr(_Ptr), _Mysize(_Size) {}

        unique_array(unique_array&& _Other) noexcept : _Myptr(_Other._Myptr), _Mysize(_Other._Mysize) {
            _Other._Myptr  = nullptr;
            _Other._Mysize = 0;
        }

        ~unique_array() noexcept {
            _Delete_array();
        }

        unique_array& operator=(unique_array&& _Other) noexcept {
            reset(_Other._Myptr, _Other._Mysize);
            _Other._Myptr  = nullptr;
            _Other._Mysize = 0;
            return *this;
        }

        unique_array& operator=(::std::nullptr_t) noexcept {
            _Delete_array();
            return *this;
        }

        unique_array(const unique_array&)            = delete;
        unique_array& operator=(const unique_array&) = delete;

        explicit operator bool() const noexcept {
            return _Myptr != nullptr && _Mysize > 0;
        }

        element_type& operator[](const size_t _Idx) const {
            if (_Idx >= _Mysize) { // index out of bounds, raise an exception
                resource_overrun::raise();
            }

            return _Myptr[_Idx];
        }

        pointer get() const noexcept {
            return _Myptr;
        }

        size_t size() const noexcept {
            return _Mysize;
        }

        struct release_result {
            pointer ptr;
            size_t size;
        };

        release_result release() noexcept {
            release_result _Result = {_Myptr, _Mysize};
            _Myptr                 = nullptr;
            _Mysize                = 0;
            return _Result;
        }

        void reset() noexcept {
            _Delete_array();
        }

        void reset(pointer _New_ptr, const size_t _New_size) noexcept {
            _Delete_array(); // delete the managed array
            _Myptr  = _New_ptr;
            _Mysize = _New_size;
        }

        void swap(unique_array& _Other) noexcept {
            ::std::swap(_Myptr, _Other._Myptr);
            ::std::swap(_Mysize, _Other._Mysize);
        }

    private:
        void _Delete_array() noexcept {
            // delete the managed array, then assign default values (null and zero)
            if (_Myptr && _Mysize > 0) {
                ::mjx::delete_object_array(_Myptr, _Mysize);
                _Myptr  = nullptr;
                _Mysize = 0;
            }
        }

        pointer _Myptr;
        size_t _Mysize;
    };

    template <class _Ty>
    inline bool operator==(const unique_array<_Ty>& _Left, const unique_array<_Ty>& _Right) noexcept {
        return _Left.get() == _Right.get() && _Left.size() == _Right.size();
    }

    template <class _Ty>
    inline bool operator==(const unique_array<_Ty>& _Left, ::std::nullptr_t) noexcept {
        return _Left.get() == nullptr && _Left.size() == 0;
    }

    template <class _Ty>
    inline ::std::strong_ordering operator<=>(
        const unique_array<_Ty>& _Left, const unique_array<_Ty>& _Right) noexcept {
        const auto& _Cmp = _Left.get() <=> _Right.get();
        if (_Cmp != ::std::strong_ordering::equal) { // pointers not equal, break
            return _Cmp;
        }

        // if the pointers are equal, then compare the sizes
        return _Left.size() <=> _Right.size();
    }

    template <class _Ty>
    inline unique_array<_Ty> make_unique_array(const size_t _Size) {
        // create a unique array that manages a newly created object array
        return unique_array<_Ty>(::mjx::create_object_array<_Ty>(_Size), _Size);
    }

    template <class _Ty>
    inline unique_array<_Ty> make_unique_array(const size_t _Size, const _Ty& _Value) {
        // create a unique array that manages a newly created object array, initialized with the given value
        _Ty* const _Ptr = ::mjx::allocate_object_array<_Ty>(_Size);
        for (size_t _Idx = 0; _Idx < _Size; ++_Idx) {
            ::mjx::construct_object(::std::addressof(_Ptr[_Idx]), _Value);
        }
        
        return unique_array<_Ty>(_Ptr, _Size);
    }

    class _MJXSDK_EXPORT reference_counter { // thread-safe reference counter
    public:
        reference_counter() noexcept;
        ~reference_counter() noexcept;

        explicit reference_counter(const long _Refs) noexcept;

        reference_counter(const reference_counter&)            = delete;
        reference_counter& operator=(const reference_counter&) = delete;

        // increments the number of references
        void increment() noexcept;
        
        // decrements the number of references
        long decrement() noexcept;
        
        // returns the number of references
        long use_count() const noexcept;

        // checks whether the number of references is exactly 1
        bool unique() const noexcept;

    private:
        ::std::atomic<long> _Myrefs;
    };

    template <class _Ty>
    class shared_ptr { // smart pointer with shared object ownership semantics
    public:
        using element_type = _Ty;
        using pointer      = _Ty*;

        shared_ptr() noexcept : _Myptr(nullptr), _Myctr(nullptr) {}

        shared_ptr(::std::nullptr_t) noexcept : _Myptr(nullptr), _Myctr(nullptr) {}

        explicit shared_ptr(pointer _Ptr) : _Myptr(_Ptr), _Myctr(::mjx::create_object<reference_counter>(1)) {}
        
        shared_ptr(const shared_ptr& _Other) noexcept : _Myptr(_Other._Myptr), _Myctr(_Other._Myctr) {
            if (_Myctr) {
                _Myctr->increment();
            }
        }

        shared_ptr(shared_ptr&& _Other) noexcept : _Myptr(_Other._Myptr), _Myctr(_Other._Myctr) {
            _Other._Myptr = nullptr;
            _Other._Myctr = nullptr;
        }

        shared_ptr(unique_ptr<_Ty>&& _Unique)
            : _Myptr(_Unique.release()), _Myctr(::mjx::create_object<reference_counter>(1)) {}

        ~shared_ptr() noexcept {
            _Release();
        }

        shared_ptr& operator=(const shared_ptr& _Other) noexcept {
            shared_ptr(_Other).swap(*this);
            return *this;
        }

        shared_ptr& operator=(shared_ptr&& _Other) noexcept {
            shared_ptr(::std::move(_Other)).swap(*this);
            return *this;
        }

        shared_ptr& operator=(unique_ptr<_Ty>&& _Unique) {
            shared_ptr(::std::move(_Unique)).swap(*this);
            return *this;
        }

        explicit operator bool() const noexcept {
            return _Myptr != nullptr;
        }

        element_type& operator*() const noexcept {
            // the behavior is undefined if the stored pointer is null
            return *_Myptr;
        }

        pointer operator->() const noexcept {
            return _Myptr;
        }

        pointer get() const noexcept {
            return _Myptr;
        }

        long use_count() const noexcept {
            return _Myctr ? _Myctr->use_count() : 0;
        }

        bool unique() const noexcept {
            return _Myctr ? _Myctr->unique() : false;
        }

        void reset() noexcept {
            shared_ptr().swap(*this);
        }

        void reset(pointer _New_ptr) noexcept {
            shared_ptr(_New_ptr).swap(*this);
        }

        void swap(shared_ptr& _Other) noexcept {
            ::std::swap(_Myptr, _Other._Myptr);
            ::std::swap(_Myctr, _Other._Myctr);
        }

    private:
        void _Release() noexcept {
            // decrement the reference counter, delete the managed pointer if the counter is zero
            if (_Myctr && _Myctr->decrement() == 0) {
                ::mjx::delete_object(_Myptr);
                ::mjx::delete_object(_Myctr);
                _Myptr = nullptr;
                _Myctr = nullptr;
            }
        }

        pointer _Myptr;
        reference_counter* _Myctr;
    };

    template <class _Ty>
    inline bool operator==(const shared_ptr<_Ty>& _Left, const shared_ptr<_Ty>& _Right) noexcept {
        return _Left.get() == _Right.get();
    }

    template <class _Ty>
    inline bool operator==(const shared_ptr<_Ty>& _Left, ::std::nullptr_t) noexcept {
        return _Left.get() == nullptr;
    }

    template <class _Ty>
    inline ::std::strong_ordering operator<=>(const shared_ptr<_Ty>& _Left, const shared_ptr<_Ty>& _Right) noexcept {
        return _Left.get() <=> _Right.get();
    }

    template <class _Ty, class... _Types>
    inline shared_ptr<_Ty> make_shared(_Types&&... _Args) {
        // create a shared pointer that manages a newly created object
        return shared_ptr<_Ty>(::mjx::create_object<_Ty>(::std::forward<_Types>(_Args)...));
    }

    template <class _Ty>
    inline shared_ptr<_Ty> make_shared(const _Ty& _Value) {
        // create a shared pointer that manages a newly created object
        return shared_ptr<_Ty>(::mjx::create_object<_Ty>(_Value));
    }

    template <class _Ty>
    inline shared_ptr<_Ty> make_shared_for_overwrite() {
        // create a shared pointer that manages a newly default-initialized object
        return shared_ptr<_Ty>(::mjx::create_object<_Ty>());
    }

    template <class _Ty>
    class shared_array { // smart pointer with shared object ownership semantics for arrays
    public:
        using element_type = _Ty;
        using pointer      = _Ty*;

        shared_array() noexcept : _Myptr(nullptr), _Mysize(0), _Myctr(nullptr) {}

        shared_array(::std::nullptr_t, const size_t) noexcept : _Myptr(nullptr), _Mysize(0), _Myctr(nullptr) {}

        explicit shared_array(pointer _Ptr, const size_t _Size)
            : _Myptr(_Ptr), _Mysize(_Size), _Myctr(::mjx::create_object<reference_counter>(1)) {}

        shared_array(const shared_array& _Other) noexcept
            : _Myptr(_Other._Myptr), _Mysize(_Other._Mysize), _Myctr(_Other._Myctr) {
            if (_Myctr) {
                _Myctr->increment();
            }
        }

        shared_array(shared_array&& _Other) noexcept
            : _Myptr(_Other._Myptr), _Mysize(_Other._Mysize), _Myctr(_Other._Myctr) {
            _Other._Myptr  = nullptr;
            _Other._Mysize = 0;
            _Other._Myctr  = nullptr;
        }

        shared_array(unique_array<_Ty>&& _Unique)
            : _Myptr(_Unique.get()), _Mysize(_Unique.size()), _Myctr(::mjx::create_object<reference_counter>(1)) {
            _Unique.release();
        }

        ~shared_array() noexcept {
            _Release();
        }

        shared_array& operator=(const shared_array& _Other) noexcept {
            shared_array(_Other).swap(*this);
            return *this;
        }

        shared_array& operator=(shared_array&& _Other) noexcept {
            shared_array(::std::move(_Other)).swap(*this);
            return *this;
        }

        shared_array& operator=(unique_array<_Ty>&& _Unique) {
            shared_array(::std::move(_Unique)).swap(*this);
            return *this;
        }

        explicit operator bool() const noexcept {
            return _Myptr != nullptr && _Mysize > 0;
        }

        element_type& operator[](const size_t _Idx) const {
            if (_Idx >= _Mysize) { // index out of bounds, raise an exception
                resource_overrun::raise();
            }

            return _Myptr[_Idx];
        }

        pointer get() const noexcept {
            return _Myptr;
        }

        size_t size() const noexcept {
            return _Mysize;
        }

        long use_count() const noexcept {
            return _Myctr ? _Myctr->use_count() : 0;
        }

        bool unique() const noexcept {
            return _Myctr ? _Myctr->unique() : false;
        }

        void reset() noexcept {
            _Release();
        }

        void reset(pointer _New_ptr, const size_t _New_size) {
            shared_array(_New_ptr, _New_size).swap(*this);
        }

        void swap(shared_array& _Other) noexcept {
            ::std::swap(_Myptr, _Other._Myptr);
            ::std::swap(_Mysize, _Other._Mysize);
            ::std::swap(_Myctr, _Other._Myctr);
        }

    private:
        void _Release() noexcept {
            // decrement the reference counter, delete the managed array if the counter is zero
            if (_Myctr && _Myctr->decrement() == 0) {
                ::mjx::delete_object_array(_Myptr, _Mysize);
                ::mjx::delete_object(_Myctr);
                _Myptr  = nullptr;
                _Mysize = 0;
                _Myctr  = nullptr;
            }
        }

        pointer _Myptr;
        size_t _Mysize;
        reference_counter* _Myctr;
    };

    template <class _Ty>
    inline bool operator==(const shared_array<_Ty>& _Left, const shared_array<_Ty>& _Right) noexcept {
        return _Left.get() == _Right.get() && _Left.size() == _Right.size();
    }

    template <class _Ty>
    inline bool operator==(const shared_array<_Ty>& _Left, ::std::nullptr_t) noexcept {
        return _Left.get() == nullptr && _Left.size() == 0;
    }

    template <class _Ty>
    inline ::std::strong_ordering operator<=>(
        const shared_array<_Ty>& _Left, const shared_array<_Ty>& _Right) noexcept {
        const auto& _Cmp = _Left.get() <=> _Right.get();
        if (_Cmp != ::std::strong_ordering::equal) { // pointers not equal, break
            return _Cmp;
        }

        // if the pointers are equal, then compare the sizes
        return _Left.size() <=> _Right.size();
    }

    template <class _Ty>
    inline shared_array<_Ty> make_shared_array(const size_t _Size) {
        // create a shared array that manages a newly created object array
        return shared_array<_Ty>(::mjx::create_object_array<_Ty>(_Size), _Size);
    }

    template <class _Ty>
    inline shared_array<_Ty> make_shared_array(const size_t _Size, const _Ty& _Value) {
        // create a shared array that manages a newly created object array, initialized with the given value
        _Ty* const _Ptr = ::mjx::allocate_object_array<_Ty>(_Size);
        for (size_t _Idx = 0; _Idx < _Size; ++_Idx) {
            ::mjx::construct_object(::std::addressof(_Ptr[_Idx]), _Value);
        }
        
        return shared_array<_Ty>(_Ptr, _Size);
    }
} // namespace mjx

#endif // _MJXSDK_SMART_POINTER_HPP_