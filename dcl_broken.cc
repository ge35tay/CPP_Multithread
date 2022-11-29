#include <mutex>
#include <iostream>
#include <atomic>
#include <vector>
#include <thread>
class some_type {
	// ...
public:
	void do_it() { /*...*/ }
};

std::atomic<some_type* > ptr{nullptr};            // Variable to be lazily initialized
std::mutex process_mutex;

void process() {
    if (!ptr) {                     // First check of ptr
        std::lock_guard<std::mutex> lk(process_mutex);
        
        if (!ptr)                  // Second check of ptr
            ptr = new some_type;   // Initialize ptr
    }
    // we change ptr with atomic, we need a new pointer of some-type
    some_type *new_ptr = ptr;
    new_ptr->do_it();
}

int main()
{
    std::vector<std::thread> threads;
    for (int i=0; i < 10; i++)
    {
        threads.push_back(std::thread{process});
    }
    for(auto &t: threads) t.join();
    return 0;
}