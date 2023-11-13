#ifdef CPPTRACE_UNWIND_WITH_WINAPI

#include <cpptrace/cpptrace.hpp>
#include "unwind.hpp"
#include "../platform/common.hpp"
#include "../platform/utils.hpp"

#include <algorithm>
#include <cstdint>
#include <vector>

#include <windows.h>

// Fucking windows headers
#ifdef min
 #undef min
#endif

namespace cpptrace {
namespace detail {
    CPPTRACE_FORCE_NO_INLINE
    std::vector<frame_ptr> capture_frames(std::size_t skip, std::size_t max_depth) {
        std::vector<void*> addrs(std::min(hard_max_frames, max_depth), nullptr);
        int n_frames = CaptureStackBackTrace(
            static_cast<ULONG>(skip + 1),
            static_cast<ULONG>(addrs.size()),
            addrs.data(),
            NULL
        );
        // I hate the copy here but it's the only way that isn't UB
        std::vector<frame_ptr> frames(n_frames, 0);
        for(std::size_t i = 0; i < n_frames; i++) {
            // On x86/x64/arm, as far as I can tell, the frame return address is always one after the call
            // So we just decrement to get the pc back inside the `call` / `bl`
            // This is done with _Unwind too but conditionally based on info from _Unwind_GetIPInfo.
            frames[i] = reinterpret_cast<frame_ptr>(addrs[i]) - 1;
        }
        return frames;
    }

    CPPTRACE_FORCE_NO_INLINE
    std::size_t safe_capture_frames(frame_ptr*, std::size_t, std::size_t, std::size_t) {
        // Can't safe trace with winapi
        return 0;
    }
}
}

#endif
