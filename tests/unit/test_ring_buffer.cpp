#include "no_alloc/containers/static_ring_buffer.hpp"
#include <iostream>
#include <cassert>
#include <thread>
#include <numeric>


void test_spsc_concurrency(){
    std::cout <<"--- Starting SPSC Thread Test ---\n";

    no_alloc::static_ring_buffer<uint64_t, 1024> rb;
    const uint64_t iterations = 1000000;
    uint64_t consumer_sum = 0;

    //NOTE: Producer thread: pushes 1 -> 1e6
    std::thread producer([&](){
        for(uint64_t i = 1; i <= iterations; ++i){
            //Spin-lock until buffer has room
            while(!rb.push(i)){
                std::this_thread::yield();
            }
        }    
    });

    //NOTE: Consumer thread: pops and sums numbers 
    std::thread consumer([&](){
        uint64_t received_count = 0;
        while(received_count < iterations){
            uint64_t val;
            if(rb.pop(val)) {
                consumer_sum += val;
                received_count++;
            } else {
                std::this_thread::yield();
            }
        }
    });

    producer.join();
    consumer.join();

    //NOTE: checking sum matches expected 
    uint64_t expected_sum = (iterations * (iterations + 1)) / 2;
    std::cout << "Consumer Sum: " << consumer_sum << '\n';
    std::cout << "Expected Sum: " << expected_sum << '\n';

    assert(consumer_sum == expected_sum);
    std::cout << "Test Passed :))) No data lost or corrupted.\n";

}


constexpr bool test_compile_time(){
    no_alloc::static_ring_buffer<int, 4> rb;
    if(!rb.empty()){return false;}

    rb.push(10); rb.push(20); rb.push(30);

    int val = 0;
    if(!rb.pop(val) || val != 10){return false;}
    if(!rb.pop(val) || val != 20){return false;}

    rb.push(40);
    rb.push(50);
    if(rb.empty()){return false;}
    return true;
}

static_assert(test_compile_time(), "Ring Buffer Compile Time Test Failed");

//NOTE: no compile-time checks necessary since threads require runtime
int main(){
    test_spsc_concurrency();
    return 0;
}
