#include <iostream>
#include <thread>
#include <string>
void hello()
{
    std::cout << "Hello, Thread! \n"; 
}

void hello2(std::string str)
{
    std::cout << str << std::endl;
}

void hello3(std::string& s)
{
    s = "xyz";
}

void hello4(std::string && s)
{
    std::cout << "Ownership of \"" << s << "\" transferred to thread"<< std::endl; 
}

class greeter{
public:
    void hello(){std::cout << "hello again thread" << std::endl;}
};

int main()
{
    std::string s{"anc"};
    std::thread t{hello};  // use the callable function name
    std::cout << "Doing some work..";
    t.join();    // tell the main function to stop
    
    std::thread t1{hello2, "Thread: Fuck off"};
    t1.join();
    
    std::thread t2{hello3, std::ref(s)};
    t2.join();
    std::cout << "string  is now " << s << std::endl;
    std::thread t3{hello4, std::move(s)};
    t3.join();
    std::cout << "string  is now " << s << std::endl;

    // to call a function in class (need a & before), we need to input the object address (like the this pointer)
    greeter greet;
    std::thread t4{ &greeter::hello, &greet};
    t4.join();
    std::cout << "Finished some work.." << std::endl;
}

// to compile 
// g++ -pthread Thread_launch.cpp