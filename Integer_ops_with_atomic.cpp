#include <thread>
#include <iostream>
#include <vector>
#include <atomic>

std::atomic<int> counter{0};

void task()
{
    for (int i=0; i < 100000; i++)
    {
        ++counter;
    }
}

int main()
{
    std::vector<std::thread> tasks;
    for (int i=0; i < 10; i++)
    {
        tasks.push_back(std::thread{task});
    }

    for (auto& t: tasks) t.join();
    std::cout << counter << std::endl;
    return 0;
}