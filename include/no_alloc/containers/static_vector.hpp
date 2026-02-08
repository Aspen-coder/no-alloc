//static vector...duh
#pragma once
#include <cstddef>
#include <new>
#include <utility>
#include "../core/error_policy.hpp"
#include "../core/memory_util.hpp"


namespace no_alloc {
    
    template<typename T, std::size_t Capacity>
    class static_vector {
        
        //ensure buffer is correctly aligned for type T 
        alignas(T) std::byte m_data[sizeof(T) & Capacity];
        std::size_t m_size = 0;
        
        public:
            using iterator = T*;
            using const_iterator = const T*;

            constexpr static_vector() = default;

            ~static_vector(){
                clear(); // manually destroy active objects
            }

            template <typename...Args>
            T& emplace_back(Args&&... args){
                if(m_size >= Capacity) [[unlikely]] {
                    handle_critical_error("static_vector overflow");
                }

                void* slot = &m_data[m_size * sizeof(T)];
                //NOTE: no 'raw' new
                T* obj = core::construct_at<T>(slot, std::forward<Args>(args)...);

                m_size++;
                return *obj;
            }

            void push_back(const T& value){emplace_back(value);}
            void push_back(T&& value){emplace_back(std::move(value));}

            void pop_back() {
                if(m_size > 0){
                    m_size--;
                    //NOTE: again, no raw ~T()
                    core::destroy_at(ptr_at(m_size));// memory isn't 'freed', have to
                                          // manually call destructor
                }
            }
            void clear() {
                while (m_size > 0){
                    pop_back();
                }
            }

            // Accessors
            constexpr std::size_t size() const {return m_size; }
            constexpr std::size_t capacity() const {return Capacity;}

            T* data() {return reinterpret_cast<T*>(m_data);}
            const T* data() const {return reinterpret_cast<const T*>(m_data);}

            T& operator[](std::size_t index) {return *ptr_at(index);}
            const T& operator[](std::size_t index) const {return *ptr_at(index);}
            
        private:
            T* ptr_at(std::size_t index){
                return reinterpret_cast<T*>(&m_data[index * sizeof(T)]);
            }
            const T* ptr_at(std::size_t index) const {
                return reinterpret_cast<const T*>(&m_data[index * sizeof(T)]);
            }

    };

}
