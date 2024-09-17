// exception.hpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef _MJXSDK_EXCEPTION_HPP_
#define _MJXSDK_EXCEPTION_HPP_
#include <mjxsdk/export.hpp>

namespace mjx {
    class _MJXSDK_EXPORT allocation_failure {
    public:
        allocation_failure() noexcept;
        ~allocation_failure() noexcept;

        // raises allocation failure exception
        [[noreturn]] static void raise();
    };

    class _MJXSDK_EXPORT allocation_limit_exceeded {
    public:
        allocation_limit_exceeded() noexcept;
        ~allocation_limit_exceeded() noexcept;

        // raises allocation limit exceeded exception
        [[noreturn]] static void raise();
    };

    class _MJXSDK_EXPORT resource_overrun {
    public:
        resource_overrun() noexcept;
        ~resource_overrun() noexcept;

        // raises resource overrun exception
        [[noreturn]] static void raise();
    };
} // namespace mjx

#endif // _MJXSDK_EXCEPTION_HPP_