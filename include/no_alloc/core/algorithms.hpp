// bin search and other algos to go with static DS  
#pragma once
#include <cstddef>

namespace no_alloc::core {
    /*
     * @brief: A constexpr binary search for sorted contiguous containers.
     * @return Index of the element if found, or static_cast<size_t>(-1)
    */
    template<typename Container, typename T>
    constexpr std::size_t binary_search(const Container& container, const T& value) {
        if(container.size() == 0){return static_cast<std::size_t>(-1);}

        std::size_t low = 0;
        std::size_t high = container.size() - 1;

        while(low <= high){
            std::size_t mid = low + (high - low) / 2;
            const auto& mid_val = container[mid];

            if(mid_val == value){
                return mid;
            } else if(mid_val < value) {
                low = mid+1; 
            } else {
                if(mid == 0){break;} //NOTE: no underflow
                high = mid-1;
            }
        }

        return static_cast<std::size_t>(-1);
    }

}
