#include "no_alloc/containers/static_vector.hpp"
#include <cassert>
#include <iostream>

struct Tracker {
    static int count;
    Tracker(){count++; std::cout << "Constructed\n";}
    ~Tracker() {count--; std::cout<<"Destroyed\n";}
};

int Tracker::count = 0;

void test_lifetime(){
    {
        std::cout << "-----Starting Scope-----\n";
        no_alloc::static_vector<Tracker, 5> vec;
        vec.emplace_back();
        vec.emplace_back();
        assert(Tracker::count == 2);
        std::cout << "Popping one...\n";
        vec.pop_back();
        assert(Tracker::count == 1);
        std::cout << "-----Ending Scope-----\n";
    }
    assert(Tracker::count == 0); // Destructor shouda cleared everything
    std::cout << "Test Passed: All objects cleaned up :)\n";
}

int main() {
    test_lifetime();
    return 0;
}
