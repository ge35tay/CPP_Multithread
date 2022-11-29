#include <iostream>
#include <future>

int main()
{
    // Packaged task with lambda as task function
    std::packaged_task<int(int, int)> ptask([](int a, int b) {return a+b; });

    std::future<int> fut = ptask.get_future();    // the future for packaged_task's promise
    std::thread t (std::move(ptask), 6, 7);       // create thread to run the task
    t.join();
    std::cout << "6 + 7 is " << fut.get() << std::endl;
    return 0;
}