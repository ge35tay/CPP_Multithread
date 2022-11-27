#include <iostream>
#include <thread>

int global_variable = 0;

void count()
{
    for (int i=0; i<100000; i++)
        global_variable++;
}


int main()
{
    std::thread t1{count};
    std::thread t2{count};
    t1.join();
    t2.join();
    std::cout << "now the result is " << global_variable << std::endl;
    return 0;
}