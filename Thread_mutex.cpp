#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <mutex>


std::mutex print_mutex;
void print(std::string str)
{
    for (int i=0; i<5; i++)
    {
        print_mutex.lock();
        std::cout << str[0] << str[1] << str[2] << std::endl;
        print_mutex.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        /*
            The output is still interleaved, but each thread's output is separate from the others
        */
    }
}


int main()
{
    // threads can be interrupted when writing, other threads can then run and write their output
    std::thread t1{print, "abc"};
    std::thread t2{print, "xyz"};
    std::thread t3{print, "def"};
    t1.join();
    t2.join();
    t3.join();
    return 0;
}