#include <iostream>
#include <thread>
#include <condition_variable>
#include <string>

std::mutex mut;
//std::condition_variable cv;

bool finished_flag=false;
std::string sdata{"Empty"};

// Waiting thread
void reader()
{
    
    std::unique_lock<std::mutex> guard(mut);  // Acquire lock
        // cv.wait(guard);                           // wait for condition variable to be notified
    while (!finished_flag)
    {
        guard.unlock();
        std::this_thread::sleep_for(std::chrono::microseconds(10));  // Allow writer thread to access data 
        guard.lock();                                                  // lock before access the data
    }
    std::cout << "Data is " << sdata << std::endl;
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
    // cv.notify_one();          // notify the condition variable
    finished_flag = true;
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