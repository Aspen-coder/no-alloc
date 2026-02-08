#pragma once
#include <cstddef>
#include <new>
#include <utility>

namespace no_alloc::core {
    
    //NOTE: construct object in specific memory slot
    template <typename T, typename... Args>
    constexpr T* construct_at(void* ptr, Args&&... args) {
        return ::new (ptr) T(std::forward<Args>(args)...);
    }

    //NOTE: destroy an object without deallocating memory
    template<typename T>
    constexpr void destroy_at(T* ptr){
        if(ptr) {
            ptr->~T();
        }
    }
}
