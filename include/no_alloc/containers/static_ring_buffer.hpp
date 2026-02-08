// single-producer single-consumer (SPSC) lock-free design

#pragma once
#include <atomic>
#include <cstddef>
#include "../core/error_policy.hpp"

namespace no_alloc {
    
    template<typename T, std::size_t Capacity>
    class static_ring_buffer {
        
        static_assert((Capacity & (Capacity - 1)) == 0, 
                "Capacity must be a power of 2");

        alignas(T) T m_data[Capacity];

        //NOTE: Separate head & tail so the Producer & Consumer don't fight over
        //the same L1 cache line (False Sharing)
        
        alignas(64) std::atomic<std::size_t> m_head{0};
        alignas(64) std::atomic<std::size_t> m_tail{0};

        public:
            constexpr static_ring_buffer() = default;

            //NOTE: for Producer
            bool push(const T& value){
                const size_t head = m_head.load(std::memory_order_relaxed);
                const size_t next_head = (head + 1) & (Capacity - 1);

                // check if full
                if(next_head == m_tail.load(std::memory_order_acquire)) [[unlikely]] {
                    return false;
                }
                
                m_data[head] = value;
                //data write before head update
                m_head.store(next_head, std::memory_order_release);
                return true;
            }

            //NOTE: for Consumer 
            bool pop(T& out_value){
                const size_t tail = m_tail.load(std::memory_order_relaxed);

                //if empty
                if(tail == m_head.load(std::memory_order_acquire)) [[unlikely]] {
                    return false;
                }
                
                out_value = m_data[tail];
                const size_t next_tail = (tail + 1) & (Capacity - 1);
                //data read before tail update
                m_tail.store(next_tail, std::memory_order_release);
                return true;
            }

    };

}
