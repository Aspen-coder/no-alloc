find_package(Threads REQUIRED)


# --- VECTOR TEST --- 
add_executable(test_vector test_vector.cpp)
target_link_libraries(test_vector PRIVATE no_alloc)
add_test(NAME VectorLifetimeTest COMMAND test_vector)

# --- Ring Buffer TEST ---
add_executable(test_ring_buffer test_ring_buffer.cpp)
target_link_libraries(test_ring_buffer PRIVATE no_alloc Threads::Threads)
add_test(NAME RingBufferTest COMMAND test_ring_buffer)

# --- Static String TEST ---
add_executable(test_static_string test_static_string.cpp)
target_link_libraries(test_static_string PRIVATE no_alloc)
add_test(NAME StaticStringTest COMMAND test_static_string)


