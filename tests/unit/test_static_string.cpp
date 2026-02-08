#include "no_alloc/containers/static_string.hpp"
#include <iostream>
#include <cassert>
#include <string_view>

void test_static_string() {
    no_alloc::static_string<32> str("Hello");
    str.append(" World");

    assert(std::string_view(str) == "Hello World");
    assert(str.size() == 11);
    assert(str.c_str()[11] == '\0');

    std::cout << "Static String Test Passed: " << str.c_str() << '\n';
}

int main(){
    test_static_string();
    return 0;
}
