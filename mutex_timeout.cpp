#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>

std::timed_mutex the_mutex;
void task1(){
    std::cout << "task1 trying to get lock" << std::endl;
    the_mutex.lock();
    std::cout << "task1 has lock" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::cout << "task1 releasing lock" << std::endl;
    the_mutex.unlock();
}

void task2()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::cout << "Task2 trying to get lock" << std::endl;
    auto deadline = std::chrono::system_clock::now() + std::chrono::milliseconds(90);

    while (!the_mutex.try_lock_until(deadline))
    {
        deadline = std::chrono::system_clock::now() + std::chrono::milliseconds(100);
        std::cout <<   "Task2 can not get lock" << std::endl;
    }
    std::cout << "task 2 has lock" << std::endl;
    the_mutex.unlock();
}

int main()
{
    std::thread t1{task1}, t2{task2};
    t1.join();
    t2.join();
    return 0;
}