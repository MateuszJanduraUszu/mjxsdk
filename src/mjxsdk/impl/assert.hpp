// assert.hpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef _MJXSDK_IMPL_ASSERT_HPP_
#define _MJXSDK_IMPL_ASSERT_HPP_
#ifdef _MJX_WINDOWS
#include <crtdbg.h>
#else // ^^^ _MJX_WINDOWS ^^^ / vvv _MJX_LINUX vvv
#include <cstdio>
#include <cstdlib>
#endif // _MJX_WINDOWS

// generic assertion macros, useful in debug mode
#ifdef _MJX_WINDOWS
#define _REPORT_ERROR(_Msg) ::_CrtDbgReport(_CRT_ERROR, __FILE__, __LINE__, nullptr, _Msg)
#else // ^^^ _MJX_WINDOWS ^^^ / vvv _MJX_LINUX vvv
#define _REPORT_ERROR(_Msg)                                                              \
    ::fprintf(stderr, "%s:%d: %s: %s\n", __FILE__, __LINE__, __PRETTY_FUNCTION__, _Msg); \
    ::abort()
#endif // _MJX_WINDOWS

#define _INTERNAL_ASSERT(_Cond, _Msg) \
    if (!(_Cond)) {                   \
        _REPORT_ERROR(_Msg);          \
    }
#endif // _MJXSDK_IMPL_ASSERT_HPP_