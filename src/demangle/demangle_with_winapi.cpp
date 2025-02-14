#ifdef CPPTRACE_DEMANGLE_WITH_WINAPI

#include "demangle/demangle.hpp"

#include <string>

#ifndef WIN32_LEAN_AND_MEAN
 #define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <dbghelp.h>

namespace cpptrace {
namespace detail {
    std::string demangle(const std::string& name, bool) {
        char buffer[500];
        auto ret = UnDecorateSymbolName(name.c_str(), buffer, sizeof(buffer) - 1, 0);
        if(ret == 0) {
            return name;
        } else {
            buffer[ret] = 0; // just in case, ms' docs unclear if null terminator inserted
            return buffer;
        }
    }
}
}

#endif
