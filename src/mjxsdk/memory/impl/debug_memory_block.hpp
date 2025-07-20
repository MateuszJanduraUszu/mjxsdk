// debug_memory_block.hpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef _MJXSDK_MEMORY_IMPL_DEBUG_MEMORY_BLOCK_HPP_
#define _MJXSDK_MEMORY_IMPL_DEBUG_MEMORY_BLOCK_HPP_
#ifdef _DEBUG
#include <cstdint>
#include <mjxsdk/core/impl/assert.hpp>
#include <mjxsdk/core/impl/utils.hpp>
#include <mjxsdk/memory/allocator.hpp>
#include <mjxsdk/memory/impl/utils.hpp>

namespace mjx {
    namespace mjxsdk_impl {
        enum class _Block_state : unsigned char {
            _Uninitialized = 0,
            _Deallocated   = 1,
            _Allocated     = 2
        };

        struct _Block_sentinel { // sentinel used to detect memory corruption
            static constexpr uint32_t _Original_value = 0xCCCC'CCCC;
            uint32_t _Value                           = _Original_value;

            constexpr bool _Is_original() const noexcept {
                return _Value == _Original_value;
            }
        };

        struct _Block_header { // basic allocation info
            size_t _Size        = 0;
            size_t _Align       = 0;
            allocator_tag _Tag  = allocator_tag::unknown;
            _Block_state _State = _Block_state::_Uninitialized;
        };

#ifdef _MJX_X64
        inline constexpr size_t _Block_header_size = 18;
#else // ^^^ _MJX_X64 ^^^ / vvv _MJX_X86 vvv
        inline constexpr size_t _Block_header_size = 10;
#endif // _MJX_X64

        struct _Block_metadata { // allocation info with corruption detection
            _Block_header _Header;
            _Block_sentinel _Underrun_sentinel; // sentinel for block underrun detection
            _Block_sentinel _Overrun_sentinel; // sentinel for block overrun detection
        };

        struct _Block_offset {
            static constexpr intptr_t _Header = 0;

            static constexpr intptr_t _Header_padding() noexcept {
                // header padding is located directly after the header
                return static_cast<intptr_t>(_Block_header_size);
            }

            static constexpr intptr_t _Underrun_sentinel(const size_t _Align) noexcept {
                // underrun sentinel is located directly before the aligned user block
                return _User_block(_Align) - sizeof(_Block_sentinel);
            }

            static constexpr intptr_t _User_block(const size_t _Align) noexcept {
                // user block is located at the next aligned address after the underrun sentinel
                return _Align_value(_Block_header_size + sizeof(_Block_sentinel), _Align);
            }

            static constexpr intptr_t _Overrun_sentinel(const size_t _Size, const size_t _Align) noexcept {
                // overrun sentinel is located directly after the user block
                return _User_block(_Align) + _Size;
            }

            static constexpr intptr_t _Block_padding(const size_t _Size, const size_t _Align) noexcept {
                // block padding (if any) is located after the overrun sentinel
                return _Overrun_sentinel(_Size, _Align) + sizeof(_Block_sentinel);
            }
        };

        constexpr size_t _Calculate_block_size(const size_t _Size, const size_t _Align) noexcept {
            // calculates block size including user block and metadata
            constexpr size_t _Sentinel_size = sizeof(_Block_sentinel);
            return _Align_value(_Block_header_size + _Sentinel_size, _Align)
                + _Align_value(_Size + _Sentinel_size, _Align);
        }

        constexpr size_t _Calculate_header_padding_size(const size_t _Align) noexcept {
            // calculates header padding size, returns zero if no padding is required
            return _Block_offset::_Underrun_sentinel(_Align) - _Block_offset::_Header_padding();
        }

        constexpr size_t _Calculate_block_padding_size(
            const size_t _Block_size, const size_t _User_size, const size_t _Align) noexcept {
            // calculates block padding size, returns zero if no padding is required
            return _Block_size - _Block_offset::_Block_padding(_User_size, _Align);
        }

        inline void* _Move_ptr(void* const _Ptr, const intptr_t _Off) noexcept {
            // moves the pointer by the given offset (forward or backward)
            return static_cast<void*>(static_cast<unsigned char*>(_Ptr) + _Off);
        }

        inline const void* _Move_ptr(const void* const _Ptr, const intptr_t _Off) noexcept {
            // moves the pointer by the given offset (forward or backward)
            return static_cast<const void*>(static_cast<const unsigned char*>(_Ptr) + _Off);
        }

        inline void _Fill_at(
            void* const _Dest, const size_t _Size, const intptr_t _Off, const int _Value) noexcept {
            // fills _Size bytes at (_Dest + _Off) with _Value
            ::memset(_Move_ptr(_Dest, _Off), _Value, _Size);
        }

        inline void _Copy_dest_at(
            void* const _Dest, const void* const _Src, const size_t _Size, const size_t _Off) noexcept {
            // copies _Size bytes from _Src to (_Dest + _Off)
            ::memcpy(_Move_ptr(_Dest, _Off), _Src, _Size);
        }

        inline void _Copy_src_at(
            void* const _Dest, const void* const _Src, const size_t _Size, const size_t _Off) noexcept {
            // copies _Size bytes from (_Src + _Off) to _Dest
            ::memcpy(_Dest, _Move_ptr(_Src, _Off), _Size);
        }

        void* _Prepare_block(void* const _Block, const size_t _Block_size,
            const size_t _User_size, const size_t _Align, const allocator_tag _Tag) noexcept {
            // embeds debug metadata into the given memory block
            constexpr size_t _Sentinel_size = sizeof(_Block_sentinel);
            const _Block_header _Header     = {_User_size, _Align, _Tag, _Block_state::_Allocated};
            const _Block_sentinel _Sentinel;
            _Copy_dest_at(_Block, &_Header, _Block_header_size, _Block_offset::_Header);
            
            size_t _Padding = _Calculate_header_padding_size(_Align);
            if (_Padding > 0) { // fill header padding with 'BF' (Block Fill)
                _Fill_at(_Block, _Padding, _Block_offset::_Header_padding(), 0xBF);
            }

            // place underrun and overrun sentinels to detect memory boundary violations
            _Copy_dest_at(
                _Block, &_Sentinel, _Sentinel_size, _Block_offset::_Underrun_sentinel(_Align));
            _Copy_dest_at(_Block, &_Sentinel,
                _Sentinel_size, _Block_offset::_Overrun_sentinel(_User_size, _Align));

            _Padding = _Calculate_block_padding_size(_Block_size, _User_size, _Align);
            if (_Padding > 0) { // fill block padding with 'BF' (Block Fill)
                _Fill_at(_Block, _Padding, _Block_offset::_Block_padding(_User_size, _Align), 0xBF);
            }

            return _Move_ptr(_Block, _Block_offset::_User_block(_Align));
        }

        void _Validate_block_state(
            void* const _Block, const void* const _Ptr, _Block_header& _Header) noexcept {
            // validates the current block state and detects invalid memory usage
            switch (_Header._State) {
            case _Block_state::_Uninitialized:
                _REPORT_ERROR("Corrupted block at 0x%p. Memory was not allocated.", _Ptr);
                break;
            case _Block_state::_Deallocated:
                _REPORT_ERROR("Corrupted block at 0x%p. Memory was already deallocated.", _Ptr);
                break;
            default:
                break;
            }

            // mark the block as deallocated to prevent double-free
            _Header._State = _Block_state::_Deallocated;
            _Copy_dest_at(_Block, &_Header, _Block_header_size, _Block_offset::_Header);
        }

        _Block_metadata _Extract_metadata(
            void* const _Block, const size_t _Size, const size_t _Align) noexcept {
            // extracts block metadata (header and sentinels) from the given memory block
            constexpr size_t _Sentinel_size = sizeof(_Block_sentinel);
            _Block_metadata _Meta;
            _Copy_src_at(&_Meta._Header, _Block, _Block_header_size, _Block_offset::_Header);
            _Copy_src_at(&_Meta._Underrun_sentinel,
                _Block, _Sentinel_size, _Block_offset::_Underrun_sentinel(_Align));
            _Copy_src_at(&_Meta._Overrun_sentinel,
                _Block, _Sentinel_size, _Block_offset::_Overrun_sentinel(_Size, _Align));
            return _Meta;
        }

        void* _Extract_and_validate_block(void* const _Ptr, const size_t _Size,
            const size_t _Align, const allocator_tag _Tag) noexcept {
            // extracts the original block from the user pointer and validates its metadata and integrity
            void* const _Block    = _Move_ptr(_Ptr, -_Block_offset::_User_block(_Align));
            _Block_metadata _Meta = _Extract_metadata(_Block, _Size, _Align);
            if (_Meta._Header._Size != _Size) { // report size mismatch
                _REPORT_ERROR("Corrupted block at 0x%p. Size is %zu, but should be %zu.",
                    _Ptr, _Size, _Meta._Header._Size);
            }

            if (_Meta._Header._Align != _Align) { // report alignment mismatch
                _REPORT_ERROR("Corrupted block at 0x%p. Alignment is %zu, but should be %zu.",
                    _Ptr, _Align, _Meta._Header._Align);
            }

            if (_Meta._Header._Tag != _Tag) { // report tag mismatch
                _REPORT_ERROR("Corrupted block at 0x%p. Tag is %u, but should be %u.",
                    _Ptr, static_cast<uint32_t>(_Tag), static_cast<uint32_t>(_Meta._Header._Tag));
            }

            _Validate_block_state(_Block, _Ptr, _Meta._Header);
            if (!_Meta._Underrun_sentinel._Is_original()) { // report sentinel corruption
                _REPORT_ERROR(
                    "Corrupted block at 0x%p. Memory was written before the begin of the block.", _Ptr);
            }

            if (!_Meta._Overrun_sentinel._Is_original()) { // report sentinel corruption
                _REPORT_ERROR(
                    "Corrupted block at 0x%p. Memory was written after the end of the block.", _Ptr);
            }

            return _Block;
        }

        template <class _AllocFn>
        inline constexpr bool _Is_nothrow_alloc_fn = noexcept(
            noexcept(::std::declval<_AllocFn>()(size_t{0}, size_t{0})));

        template <class _DeallocFn>
        inline constexpr bool _Is_nothrow_dealloc_fn = noexcept(
            noexcept(::std::declval<_DeallocFn>()(nullptr, size_t{0}, size_t{0})));

        template <class _AllocFn>
        inline void* _Allocate_debug_block(size_t _Size, size_t _Align, const allocator_tag _Tag,
            _AllocFn&& _Alloc) noexcept(_Is_nothrow_alloc_fn<_AllocFn>) {
            // allocates and prepares a debug block with alignment and metadata
            _Align                   = _Get_effective_alignment(_Align);
            _Size                    = _Align_value(_Size, _Align);
            const size_t _Block_size = _Calculate_block_size(_Size, _Align);
            void* const _Block       = _Alloc(_Block_size, _Align);
            if (!_Block) { // allocation failed, break
                return nullptr;
            }

            return _Prepare_block(_Block, _Block_size, _Size, _Align, _Tag);
        }

        template <class _DeallocFn>
        inline void _Deallocate_debug_block(void* const _Ptr, size_t _Size, size_t _Align,
            const allocator_tag _Tag, _DeallocFn&& _Dealloc) noexcept(_Is_nothrow_dealloc_fn<_DeallocFn>) {
            // extracts and frees a debug block with alignment and metadata
            _Align = _Get_effective_alignment(_Align);
            _Size  = _Align_value(_Size, _Align);
            _Dealloc(_Extract_and_validate_block(_Ptr, _Size, _Align, _Tag),
                _Calculate_block_size(_Size, _Align), _Align);
        }
    } // namespace mjxsdk_impl
} // namespace mjx

#endif // _DEBUG
#endif // _MJXSDK_MEMORY_IMPL_DEBUG_MEMORY_BLOCK_HPP_