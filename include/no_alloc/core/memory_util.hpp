#pragma once
#include <cstddef>
#include <memory>
#include <utility>

namespace no_alloc::core {
    
    //NOTE: construct object in specific memory slot
    template <typename T, typename... Args>
    constexpr T* construct_at(T* ptr, Args&&... args) {
        return std::construct_at(ptr, std::forward<Args>(args)...);
    }

    //NOTE: destroy an object without deallocating memory
    template<typename T>
    constexpr void destroy_at(T* ptr){
        if(ptr) {
            std::destroy_at(ptr);
        }
    }
}
