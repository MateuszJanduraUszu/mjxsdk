// memory_resource.cpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#include <cstring>
#include <mjxsdk/memory/impl/global_allocator.hpp>
#include <mjxsdk/memory/impl/utils.hpp>
#include <mjxsdk/memory/memory_resource.hpp>
#include <utility>

namespace mjx {
    memory_resource::memory_resource() noexcept : _Myptr(nullptr), _Mysize(0) {}

    memory_resource::memory_resource(const memory_resource& _Other) : _Myptr(nullptr), _Mysize(0) {
        _Copy_resource(_Other._Myptr, _Other._Mysize);
    }

    memory_resource::memory_resource(memory_resource&& _Other) noexcept : _Myptr(nullptr), _Mysize(0) {
        _Move_resource(_Other._Myptr, _Other._Mysize);
    }

    memory_resource::memory_resource(const size_type _Size)
        : _Myptr(mjxsdk_impl::_Get_internal_allocator().allocate(_Size)), _Mysize(_Size) {}

    memory_resource::~memory_resource() noexcept {
        destroy();
    }

    memory_resource& memory_resource::operator=(const memory_resource& _Other) {
        if (this != &_Other) {
            destroy();
            _Copy_resource(_Other._Myptr, _Other._Mysize);
        }

        return *this;
    }

    memory_resource& memory_resource::operator=(memory_resource&& _Other) noexcept {
        if (this != &_Other) {
            destroy();
            _Move_resource(_Other._Myptr, _Other._Mysize);
        }

        return *this;
    }

    void memory_resource::_Copy_resource(const_pointer _Ptr, const size_type _Size) {
        _Myptr  = mjxsdk_impl::_Get_internal_allocator().allocate(_Size);
        _Mysize = _Size;
        ::memcpy(_Myptr, _Ptr, _Size);
    }

    void memory_resource::_Move_resource(pointer& _Ptr, size_type& _Size) noexcept {
        _Myptr  = _Ptr;
        _Mysize = _Size;
        _Ptr    = nullptr;
        _Size   = 0;
    }

    bool memory_resource::empty() const noexcept {
        return _Myptr == nullptr && _Mysize == 0;
    }

    memory_resource::pointer memory_resource::data() noexcept {
        return _Myptr;
    }

    memory_resource::const_pointer memory_resource::data() const noexcept {
        return _Myptr;
    }

    memory_resource::size_type memory_resource::size() const noexcept {
        return _Mysize;
    }

    bool memory_resource::contains(const_pointer _Block, const size_type _Size) const noexcept {
        if (empty() || !_Block || _Size == 0) {
            return false;
        }

        const_pointer _Base_end  = mjxsdk_impl::_Adjust_address_by_offset(_Myptr, _Mysize);
        const_pointer _Block_end = mjxsdk_impl::_Adjust_address_by_offset(_Block, _Size);
        return mjxsdk_impl::_Is_within_memory_block(_Myptr, _Base_end, _Block, _Block_end);
    }

    void memory_resource::swap(memory_resource& _Other) noexcept {
        ::std::swap(_Myptr, _Other._Myptr);
        ::std::swap(_Mysize, _Other._Mysize);
    }

    memory_resource::release_result memory_resource::release() noexcept {
        const release_result _Result = {_Myptr, _Mysize};
        _Myptr                       = nullptr;
        _Mysize                      = 0;
        return _Result;
    }

    void memory_resource::destroy() noexcept {
        if (_Myptr && _Mysize > 0) {
            mjxsdk_impl::_Get_internal_allocator().deallocate(_Myptr, _Mysize);
            _Myptr  = nullptr;
            _Mysize = 0;
        }
    }

    bool operator==(const memory_resource& _Left, const memory_resource& _Right) noexcept {
        return _Left.data() == _Right.data() && _Left.size() == _Right.size();
    }
} // namespace mjx