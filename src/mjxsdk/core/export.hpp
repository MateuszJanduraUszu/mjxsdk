// export.hpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef _MJXSDK_CORE_EXPORT_HPP_
#define _MJXSDK_CORE_EXPORT_HPP_

#ifdef _MJX_WINDOWS
// Windows libraries (DLLs) require an additional attribute
#ifdef _MJXSDK_BUILD
#define _MJXSDK_EXPORT __declspec(dllexport)
#else // ^^^ _MJXSDK_BUILD ^^^ / vvv !_MJXSDK_BUILD vvv
#define _MJXSDK_EXPORT __declspec(dllimport)
#endif // _MJXSDK_BUILD
#else // ^^^ _MJX_WINDOWS ^^^ / vvv _MJX_LINUX vvv
#define _MJXSDK_EXPORT
#endif // _MJX_WINDOWS
#endif // _MJXSDK_CORE_EXPORT_HPP_