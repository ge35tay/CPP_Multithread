#include <iostream>
#include <thread>
#include <condition_variable>
#include <string>

std::mutex mut;
std::condition_variable cv;
std::string sdata{"Empty"};

// Waiting thread
void reader()
{
    std::unique_lock<std::mutex> guard(mut);  // Acquire lock
    cv.wait(guard);                           // wait for condition variable to be notified
    std::cout << "Data is " << sdata << std::endl; // wake up and use the new value
}


// Modifying thread
void writer()
{
    std::cout << "Writing data... " << std::endl;
    {
        std::lock_guard<std::mutex> lg(mut);
        std::this_thread::sleep_for(std::chrono::seconds(2));
        sdata = "Populated";
    }
    cv.notify_one();          // notify the condition variable
}

int main()
{
    std::cout << "Data is " << sdata << std::endl;
    std::thread write{writer};
    std::thread read{reader};
    write.join();
    read.join();
    return 0;
}