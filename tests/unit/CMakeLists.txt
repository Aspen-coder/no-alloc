add_executable(test_vector test_vector.cpp)

target_link_libraries(test_vector PRIVATE no_alloc)

add_test(NAME VectorLifetimeTest COMMAND test_vector)
