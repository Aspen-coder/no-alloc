#include "no_alloc/containers/static_vector.hpp"
#include "no_alloc/core/algorithms.hpp"
#include <iostream>
#include <cassert>


void test_runtime_search() {
    no_alloc::static_vector<int, 10> vec;
    vec.push_back(10);
    vec.push_back(20);
    vec.push_back(30);
    vec.push_back(40);

    std::size_t idx = no_alloc::core::binary_search(vec, 30);
    assert(idx == 2);

    std::size_t not_found = no_alloc::core::binary_search(vec, 99);
    assert(not_found == static_cast<std::size_t>(-1));

    std::cout <<"Runtime binary search: PASSED\n";
}

constexpr bool check_compile_time() {
    no_alloc::static_vector<int, 5> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    return no_alloc::core::binary_search(v, 2) == 1;
}

static_assert(check_compile_time(), "Binary search must work at compile time!");
//TODO: Probably do compile time checks for other test :/ 


int main(){
    test_runtime_search();
    std::cout<< "All Algorithms Tests Passed :))!!)!\n";
    return 0;
}
