#include <iostream>
#include <thread>
#include <random>
#include <chrono>


// thread_local std::mt19937 mt;  // algorithm for generating random numberm
std::mt19937 mt;  // if thread_local these 2 thread will generate the same numbers because
                    // each thread has its own instance of engine, which is not seeded, each thread is going to print out the first 10
                    //numbers from the random engine's sequence
                    // if not they use the same seed, they will generate the first 20 numbers
void func()
{
    std::uniform_real_distribution<double> dist(0, 1); // Doubles in range 0 to 1
    for (int i = 0; i < 10; i++)
    {
        std::cout << dist(mt) << ", ";
    }
}

int main()
{
    std::cout << "Thread 1's random values: " << std::endl;
    std::thread t1{func};
    t1.join();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::cout << "\n Thread2's random values: " << std::endl;
    std::thread t2{func};
    t2.join();
    std::cout << std::endl;
    return 0;
}