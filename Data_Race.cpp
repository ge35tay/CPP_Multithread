#include <iostream>
#include <string>
#include <thread>
#include <chrono>

void print(std::string str)
{
    for (int i=0; i<5; i++)
    {
        std::cout << str[0] << str[1] << str[2] << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
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