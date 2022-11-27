#include <iostream>
#include <thread>
#include <condition_variable>
#include <string>

std::mutex mut;
std::condition_variable cv;
std::string sdata{"Empty"};
bool condition{false};            // Flag
// Waiting thread
void reader()
{
    std::cout << "Reading thread starts" << std::endl;
    std::unique_lock<std::mutex> guard(mut);  // Acquire lock
    cv.wait(guard);
    // cv.wait(guard, []{return condition;});                           // wait for condition variable to be notified
    std::cout << "Data is " << sdata << std::endl; // wake up and use the new value
}


// Modifying thread
void writer()
{
    std::cout << "Writing data... " << std::endl;
    {
        std::lock_guard<std::mutex> lg(mut);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        sdata = "Populated";
        // condition = true;
    }
    cv.notify_one();          // notify the condition variable
    std::cout << "Writing thread exits" << std::endl;
}

int main()
{
    std::cout << "Data is " << sdata << std::endl;
    std::thread write{writer};
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::thread read{reader};
    write.join();
    read.join();
    return 0;
}