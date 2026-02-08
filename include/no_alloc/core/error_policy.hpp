// panic handlers instead of exceptions
#pragma once
#include <cstdlib>

namespace no_alloc {
    
    //NOTE: no name for msg
    [[noreturn]] inline void handle_critical_error(const char* /*msg*/ ){
        //NOTE: log to hardware register??
        std::abort();
    }
}
