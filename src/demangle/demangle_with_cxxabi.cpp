#include "utils/microfmt.hpp"
#ifdef CPPTRACE_DEMANGLE_WITH_CXXABI

#include "demangle/demangle.hpp"

#include "utils/utils.hpp"

#include <cxxabi.h>

#include <cstdlib>
#include <functional>
#include <string>

namespace cpptrace {
namespace detail {
    std::string demangle(const std::string& name) {
        // https://itanium-cxx-abi.github.io/cxx-abi/abi.html#demangler
        // check both _Z and __Z, apple prefixes all symbols with an underscore
        if(!(starts_with(name, "_Z") || starts_with(name, "__Z"))) {
            return name;
        }
        // Apple clang demangles __Z just fine but gcc doesn't, so just offset the leading underscore
        std::size_t offset = 0;
        if(starts_with(name, "__Z")) {
            offset = 1;
        }
        // Mangled names don't have spaces, we might add a space and some extra info somewhere but we still want it to
        // be demanglable. Look for a space, if there is one swap it with a null terminator briefly.
        auto end = name.find(' ');
        std::string name_copy;
        std::reference_wrapper<const std::string> to_demangle = name;
        std::string rest;
        if(end != std::string::npos) {
            name_copy = name.substr(0, end);
            rest = name.substr(end);
            to_demangle = name_copy;
        }
        // presumably thread-safe
        // it appears safe to pass nullptr for status however the docs don't explicitly say it's safe so I don't
        // want to rely on it
        int status;
        char* const demangled = abi::__cxa_demangle(to_demangle.get().c_str() + offset, nullptr, nullptr, &status);
        // demangled will always be nullptr on non-zero status, and if __cxa_demangle ever fails for any reason
        // we'll just quietly return the mangled name
        if(demangled) {
            // TODO: raii_wrap the char*?
            std::string str = demangled;
            std::free(demangled);
            if(!rest.empty()) {
                str += rest;
            }
            return str;
        } else {
            return name;
        }
    }
}
}

#endif
