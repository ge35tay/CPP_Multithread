#include "singleton_class.h"
#include <thread>
#include <vector>

void task()
{
    singleton& s = get_singleton();
    std::cout << &s << std::endl;  // see the address so that we know it is the same object
}

int main()
{
    std::vector<std::thread> threads;
    for (int i=0; i < 10; i++)
    {
        threads.push_back(std::thread{task});
    }
    for (auto& t: threads)
    {
        t.join();
    }
    return 0;
}