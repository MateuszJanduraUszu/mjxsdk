// version.hpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef _MJXSDK_VERSION_HPP_
#define _MJXSDK_VERSION_HPP_

// Note: Encodes a version number from major, minor, and patch components into a 64-bit integer.
//       The major version consumes 16 bits, while the minor and patch versions each consume 24 bits.
#define _MJX_ENCODE_VERSION(_Major, _Minor, _Patch) ((_Major << 48ULL) | (_Minor << 24ULL) | _Patch)

// decodes the major, minor, and patch version numbers from a 64-bit encoded version
#define _MJX_DECODE_VERSION_MAJOR(_Version) (_Version >> 48ULL)
#define _MJX_DECODE_VERSION_MINOR(_Version) ((_Version & 0x0000'FFFF'FF00'0000ULL) >> 24ULL)
#define _MJX_DECODE_VERSION_PATCH(_Version) (_Version & 0x0000'0000'00FF'FFFFULL)

// defines the latest MJXSDK library version, synchronized with the version specified in 'res/mjxsdk.rc'
#define _MJXSDK_VERSION_MAJOR 1ULL
#define _MJXSDK_VERSION_MINOR 0ULL
#define _MJXSDK_VERSION_PATCH 0ULL
#define _MJXSDK_VERSION       _MJX_ENCODE_VERSION( \
    _MJXSDK_VERSION_MAJOR, _MJXSDK_VERSION_MINOR, _MJXSDK_VERSION_PATCH)

// checks whether the current version is greater than or equal to the specified version
#define _MJXSDK_VERSION_SUPPORTED(_Major, _Minor, _Patch) \
    (_MJXSDK_VERSION >= _MJX_ENCODE_VERSION(_Major, _Minor, _Patch))
#endif // _MJXSDK_VERSION_HPP_