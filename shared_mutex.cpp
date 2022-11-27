#include <iostream>
#include <vector>
#include <thread>
#include <shared_mutex>
#include <chrono>
#include <mutex>

std::shared_timed_mutex the_mutex;

void write(int i)
{
    std::lock_guard<std::shared_timed_mutex> lg(the_mutex);
    std::cout << "Write thread " << i << " with exclusive lock" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(50));
}

void read(int i)
{
    std::shared_lock<std::shared_timed_mutex> sl(the_mutex);
    std::cout << "read thread " << i << " with shared lock" << std::endl;
}

int main()
{
    std::vector<std::thread> threads;
    for (int i=0; i < 10; i++)
    {
        threads.push_back(std::thread{read, i});
    }
    threads.push_back(std::thread(write, 11));

    for (int i=0; i < 10; ++i){
        threads.push_back(std::thread{read, i+11});
    }

    for (auto& t : threads)
    {
        t.join();
    }
    return 0;
}