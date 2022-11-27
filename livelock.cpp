#include <thread>
#include <mutex>
#include <iostream>

std::timed_mutex mutex1, mutex2;

void func1()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    bool locked{false};

    while(!locked)
    {
        std::lock_guard<std::timed_mutex> lk(mutex1); //lock mutex1
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "After you, Claude! " << std::endl;
        locked = mutex2.try_lock_for(std::chrono::milliseconds(10));   // try to get a lock in mutex 2
    }
    if (locked)
        std::cout << "Thread 1 has locked both mutexes" << std::endl;
}

void func2()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    bool locked{false};

    while(!locked)
    {
        std::lock_guard<std::timed_mutex> lk(mutex2); //lock mutex1
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "No after you, Cecil! " << std::endl;
        locked = mutex1.try_lock_for(std::chrono::milliseconds(10));  // try to get a lock in mutex 1
    }
    if (locked)
        std::cout << "Thread2 has locked both mutexes" << std::endl;
}


int main()
{
    std::thread t1{func1};
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    std::thread t2{func2};
    t1.join();
    t2.join();
    return 0;
}