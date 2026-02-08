// fixed size string cus alloc forbidden
#pragma once
#include <cstddef>
#include <string_view>
#include <algorithm>
#include "../core/error_policy.hpp"

namespace no_alloc {
    template<std::size_t Capacity>
    class static_string {
        
        char m_data[Capacity + 1] = {0}; // +1 for null terminator
        std::size_t m_len = 0;

        public:
            constexpr static_string() = default;

            //NOTE: from string literal
            constexpr static_string(const char* s){
                assign(s);
            }

            constexpr void assign(std::string_view sv){
                if(sv.size() > Capacity) [[unlikely]] {
                    handle_critical_error("static_string overflow");
                }
                m_len = sv.size();
                for(std::size_t i = 0; i < m_len; ++i){
                    m_data[i] = sv[i];
                }
                m_data[m_len] = '\0';
            }

            constexpr void append(std::string_view sv){
                if(m_len + sv.size() > Capacity) [[unlikely]] {
                    handle_critical_error("static_string overflow");
                }
                for(std::size_t i = 0; i < sv.size(); ++i){
                    m_data[m_len + i] = sv[i];
                }
                m_len += sv.size();
                m_data[m_len] = '\0';
            }
            
            // zero-copy reading
            constexpr operator std::string_view() const {
                return {m_data, m_len};
            }

            constexpr const char* c_str() const {return m_data;}
            constexpr std::size_t size() const {return m_len;}

    };
}
