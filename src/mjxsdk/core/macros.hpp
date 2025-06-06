// macros.hpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef _MJXSDK_CORE_MACROS_HPP_
#define _MJXSDK_CORE_MACROS_HPP_

#ifdef _MJX_MSVC
#define _MJX_NOVTABLE __declspec(novtable)
#else // ^^^ MSVC ^^^ / vvv Clang or GCC vvv
#define _MJX_NOVTABLE
#endif // _MJX_MSVC
#endif // _MJXSDK_CORE_MACROS_HPP_