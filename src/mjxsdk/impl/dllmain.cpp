// dllmain.cpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#ifdef _MJX_WINDOWS
#include <mjxsdk/impl/tinywin.hpp>

// Note: Unlike Linux shared objects (SOs), Windows dynamic-link libraries (DLLs) require
//       the definition of an entry point (DllMain), even if it does nothing.

int __stdcall DllMain(HMODULE _Module, unsigned long _Reason, void*) {
    if (_Reason == DLL_PROCESS_ATTACH) {
        ::DisableThreadLibraryCalls(_Module);
    }

    return 1;
}
#endif // _MJX_WINDOWS