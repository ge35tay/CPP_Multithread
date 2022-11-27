#include <iostream>
#include <thread>
#include <chrono>

void hello(int num)
{
    std::this_thread::sleep_for(std::chrono::seconds(num));   
    std::cout << "Hello from my thread " << num << std::endl;
}

int main()
{
    std::thread t{ hello, 1};
    std::thread t2{hello, 2};
    std::thread t3{hello, 3};             // first thread will sleep 1 second and print
                                        // second thread sleep for another second
                                        // third thread sleep for another second
    t.join();
    t2.join();
    t3.join();
}