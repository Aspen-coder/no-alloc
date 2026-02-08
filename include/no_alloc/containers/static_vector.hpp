//static vector...duh
#pragma once
#include <cstddef>
#include <new>
#include <utility>
#include <type_traits>
#include "../core/error_policy.hpp"
#include "../core/memory_util.hpp"


namespace no_alloc {
    
    template<typename T, std::size_t Capacity>
    class static_vector {
        
        union Storage {
            T buffer[Capacity];
            
            constexpr Storage(){}
            constexpr ~Storage() {}
        };

        Storage m_storage;
        std::size_t m_size = 0;
        
        public:
            using iterator = T*;
            using const_iterator = const T*;

            constexpr static_vector() = default;

            constexpr ~static_vector(){
                clear(); // manually destroy active objects
            }

            template <typename...Args>
            constexpr T& emplace_back(Args&&... args){
                if(m_size >= Capacity) [[unlikely]] {
                    handle_critical_error("static_vector overflow");
                }

                T* slot = ptr_at(m_size);
                //NOTE: no 'raw' new
                T* obj = core::construct_at<T>(slot, std::forward<Args>(args)...);

                m_size++;
                return *obj;
            }

            constexpr void push_back(const T& value){emplace_back(value);}
            constexpr void push_back(T&& value){emplace_back(std::move(value));}

            constexpr void pop_back() {
                if(m_size > 0){
                    m_size--;
                    //NOTE: again, no raw ~T()
                    core::destroy_at(ptr_at(m_size));// memory isn't 'freed', have to
                                          // manually call destructor
                }
            }
            constexpr void clear() {
                while (m_size > 0){
                    pop_back();
                }
            }

            // Accessors
            constexpr std::size_t size() const {return m_size; }
            constexpr std::size_t capacity() const {return Capacity;}

            constexpr T* data() {
                return m_storage.buffer;
            }
            constexpr const T* data() const {
                return m_storage.buffer;
            }

            constexpr T& operator[](std::size_t index) {return *ptr_at(index);}
            constexpr const T& operator[](std::size_t index) const {return *ptr_at(index);}
            
        private:
            constexpr T* ptr_at(std::size_t index){
                return data() + index;
            }
            constexpr const T* ptr_at(std::size_t index) const {
                return data() + index;
            }

    };

}
