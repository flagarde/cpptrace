#ifdef CPPTRACE_GET_SYMBOLS_WITH_NOTHING

#include <cpptrace/cpptrace.hpp>
#include "symbols.hpp"

#include <vector>

namespace cpptrace {
namespace detail {
namespace nothing {
    std::vector<stacktrace_frame> resolve_frames(const std::vector<uintptr_t>& frames) {
        return std::vector<stacktrace_frame>(frames.size(), null_frame);
    }

    std::vector<stacktrace_frame> resolve_frames(const std::vector<object_frame>& frames) {
        return std::vector<stacktrace_frame>(frames.size(), null_frame);
    }
}
}
}

#endif
