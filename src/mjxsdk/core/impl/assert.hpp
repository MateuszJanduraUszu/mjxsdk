// assert.hpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef _MJXSDK_CORE_IMPL_ASSERT_HPP_
#define _MJXSDK_CORE_IMPL_ASSERT_HPP_
#ifdef _MJX_WINDOWS
#include <crtdbg.h>
#else // ^^^ _MJX_WINDOWS ^^^ / vvv _MJX_LINUX vvv
#include <cstdio>
#include <cstdlib>
#endif // _MJX_WINDOWS

// generic assertion macros, useful in debug mode
#ifdef _MJX_WINDOWS
#define _REPORT_ERROR(_Fmt, ...) \
    ::_CrtDbgReport(_CRT_ERROR, __FILE__, __LINE__, nullptr, _Fmt, __VA_ARGS__)
#else // ^^^ _MJX_WINDOWS ^^^ / vvv _MJX_LINUX vvv
#define _REPORT_ERROR(_Fmt, ...)                                               \
    ::fprintf(stderr, "%s:%d: %s: ", __FILE__, __LINE__, __PRETTY_FUNCTION__); \
    ::fprintf(stderr, _Fmt, __VA_ARGS__);                                      \
    ::abort()
#endif // _MJX_WINDOWS

#define _INTERNAL_ASSERT(_Cond, _Fmt, ...) \
    if (!(_Cond)) {                        \
        _REPORT_ERROR(_Fmt, __VA_ARGS__);  \
    }
#endif // _MJXSDK_CORE_IMPL_ASSERT_HPP_