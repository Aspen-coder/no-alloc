// single-producer single-consumer (SPSC) lock-free design

#pragma once
#include <atomic>
#include <cstddef>
#include <type_traits>

namespace no_alloc {
    template<typename T, std::size_t Capacity>
    class static_ring_buffer {
        static_assert((Capacity & (Capacity-1)) == 0, 
                "Capacity must be a power of 2");
        
        alignas(T) T m_data[Capacity];
        
        union {
            std::atomic<std::size_t> m_head;
            std::size_t m_head_raw;
        };
        union {
            std::atomic<std::size_t> m_tail;
            std::size_t m_tail_raw;
        };

        //NOTE: Internal helpers
        constexpr std::size_t get_head() const noexcept {
            if(std::is_constant_evaluated())
                return m_head_raw;
            return m_head.load(std::memory_order_relaxed);
        }
        constexpr std::size_t get_tail() const noexcept {
            if(std::is_constant_evaluated())
                return m_tail_raw;
            return m_tail.load(std::memory_order_relaxed);
        }
        public:
            constexpr static_ring_buffer() : m_head_raw(0), m_tail_raw(0) {
                if(!std::is_constant_evaluated()){
                    m_head.store(0, std::memory_order_relaxed);
                    m_tail.store(0, std::memory_order_relaxed);
                }
            }

            [[nodiscard]] constexpr bool empty() const noexcept {
                return get_head() == get_tail();
            }

            constexpr bool push(const T& value) {
                const std::size_t head = get_head();
                const std::size_t next_head = (head + 1) & (Capacity - 1);

                // check if full
                std::size_t current_tail;
                if(std::is_constant_evaluated()) {
                    current_tail = m_tail_raw;
                } else {
                    current_tail = m_tail.load(std::memory_order_acquire);
                }
                
                if(next_head == current_tail) [[unlikely]] return false;

                m_data[head] = value;

                if(std::is_constant_evaluated()){
                    m_head_raw = next_head;
                } else {m_head.store(next_head, std::memory_order_release);}

                return true;
            }

            constexpr bool pop(T& out_value) {
                const std::size_t tail = get_tail();

                //if empty 
                std::size_t current_head;
                if(std::is_constant_evaluated()){
                    current_head = m_head_raw;
                } else {
                    current_head = m_head.load(std::memory_order_acquire);
                }

                if(tail == current_head) [[unlikely]] return false;

                out_value = m_data[tail];
                const std::size_t next_tail = (tail + 1) & (Capacity - 1);

                if(std::is_constant_evaluated()){
                    m_tail_raw = next_tail;
                } else {
                    m_tail.store(next_tail, std::memory_order_release);
                }
                return true;
            }
    };
}
