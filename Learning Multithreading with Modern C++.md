# Learning Multithreading with Modern C++

## 1. Modern C++ Overview

Features

1. **Universal Initialization**

   ```c++
   int x{7};
   ```

   is equal to

   ```c++
   int x = 7;
   ```

   but in this way narrowing conversions are not allowed, e.g

   ```c++
   int y{7.7}
   ```

   is not allowed 

   Universal initialization can be used with multiple initial values

   ``` c++
   vector<int> v{4, 2, 3, 5, 1}
   ```

2. **nullptr**

   qual to NULL

3. **std::chrono library**

   Type to represent time intervals in <chrono>

   ```c++
   std::chrono::seconds   // 1 second
   std::chrono::milliseconds // 1/1000 second
   std::chrono::microseconds // 1/1000000 second
       
   std::chrono::seconds(2)     // 2 sencond interval
   ```

   C++ 14 provides literals for these time intervals in the std::chrono::literals namesapce

   ```c++
   using namespace std::literals;
   2s
   20ms
   50us
   ```

4. **Automatic Type Deduction**

   The auto type specifier will cause the complier to substitute the correct type

   ```c++
   auto x{6}
   ```

   Useful with complicated types:

   ```c++
   vector<string> v;
   auto it=v.begin()    // equivalent to vector<string>::iterator it = v.begin()
   ```

   auto will only give the underlying type, const reference etc are ignored, e.g

   ```c++
   const int& func();
   
   auto i = func()
   ```

   then i will be an int, not const int. If we need them, we need to add them ourselves

5. **Range for loops**

   Special syntax for iterating over containers

   ```c++
   for (auto it : v)
   	cout << it << ", "
   ```

   note that we need to use a reference to modify the elemets, otherwise it was a copy of elements, not element itself

   ```c++
   for (auto& it : v)
   	it += 2
   ```

6. **Lambda Expression**

   A lambda expression allow us to write source code inside other code, without having to create a separate function definition

   Lambda expressions are anonymous functions which are defined inline.

   To define a lambda expression, we put [] for the function name, (means not function name)

   In c++11, if the body is a single statement that returns a value, the compiler will deduce the return type

   ```c++
   [](int arg){return 2*arg}
   ```

   Otherwise it has to be specified with a trailing return type

   ```c++
   [](int arg)-> int{arg=2*arg; return arg}
   ```

   In c++14 the compiler can always deduce the return type

   ```c++
   auto n_even = count_if(v.begin(), v.end(), 
   [](int n){
   	return (n% 2==0);
   }
   );
   ```

   if a lambda expression needs to use a local variable, it must capture it

   ```c++
   int x{42};
   [x](){cout << x << endl;}
   ```

   To modify the local variablem, it must capture before reference

   ```c++
   [&x](){++x;}
   ```

   we can also capture all variables in the current scope

   ```c++
   int x{42};
   int y{99};
   [x](){cout << x << endl;}                    // capture specific variables
   [=](){cout << x << y << endl;}              // Captrure all local variables by value
   [&](){++x; ++y}								// Capture all local variables by reference
   ```

   To access the data members, we captures "this"

   ```c++
   [this](){cout << m1 << m2 << endl;}
   ```

   The object is captured as a reference to *this, which allows the lambda expressions to modify the data members

   ```c++
   [this](){++m1; ++m2}c
   ```

7. **Move semantics**

   optimization for copying objects, in some cases the contents of the source object can be moved into the target instead of copying them

   - lvalue and rvalue

     A value can either be an lvalue or an rvalue

     In C, an lvalue can appear on the left of the assignment, whereas an rvalue can only appear on the right

     ```c++
     x = 2;               // x is an lvalue, 2 is an rvalue
     x = func();			// x is an lvalue, func() is an rvalue
     ```

     In c++, it is more complicated. An lvalue can have its address taken using the & operator, while an rvalue cannot

     - So in c++, if want to use a & reference, the compiler will arrange it so that we create a pointer, and every time we use this reference, the compiler will arrange for the pointer to be dereferenced. So this acquires that the reference variable should have an address (lvalue).

     - There is also rvalue reference

       This is a syntactic feature which indicates that a function argument is an rvalue

       An rvalue reference has && after the type

       ```c++
       void func(int&& x);
       
       int y{2};
       // func(y)   // can not compile since y is lvalue
       func(2)		// good
       func(std::move(y))
       ```

       how can we manage this?

       Here actually the function in C++ applied for this, its called std::move() , move takes the argument y and it will return the y cast to an rvalue reference

       so instead of copying it to the function using lvalue variable, we can directly use the rvalue to save the place, **after the function returns, this variable will be empty and not size any more**. 

     - lvalue and rvalue can be used in function overload

   - Move-only types

     C++ has some types which can not be copied but can be moved

     -  unique_ptr  ("smart pointer" introduced in C++11)\
     - fstream
     - Classes used in multithreading

     These types own a resource, the object acquires ownership in the constructor, the object releases owner ship in the deconstructor

     **Only one object can own a given resource at a time**

     **The ownership of the resource can be transfered from one object to another using move semantics**

   - Move operators

     C++11 added 2 special member functions

     These are overloads of the copy constructor and assignment operator which take the rvalue reference

     ```c++
     myclass(myclas&& other) noexcept;                   // move constructor
     myclass& operator=(myclass&& other) noexcept;		// move assignment operator
     ```

     Note that the other argument is not const (because it will be moved)

8. **Making a class uncopyable**

   Before C++11, we could prevent objects from being copied by making the copy constructor and assignment operator private

   ```c++
   class test{
   private:
   	test(const test&);
   	test& operator=(const test&);
   public:
   	test(){}
   };
   ```

   In c++11, special member functions can be deleted to prevent an object from being copied

   ```c++
   test(const test& other) = delete;
   ```

   Special member function can also be defaulted, this causes the compiler to generate a default member function

   ```c++
   test(const test& other) = default;    // default copy constructor
   ```





## 2. Concurrency

Concurrency means being able to perform 2 or more activities at the same time

Disadvantage of task switching

- Every time a task switch occurs, the operating system has to perform a context switch
  - Save the current CPU state for the stopping task
    - Current value of cpu registers, program's instruction pointer etc
  - Load the saved CPU state of the starting task
    - Values of CPU registers when stopped, etc
    - May also have to reload the program's instructions and data into cache
- The cpu can not execute any instructions during the context switch





Hardware concurrency

- Most modern computers have multiple processing units (cores)
- This allows different activities to be performed on different processing units at the same time, even within the same program
- There is one hardware thread per CPU core

Software concurrency

- Modern operating system provide support for a program to perform multiple activities at the same time
- Typically there are many more software threads than hardware threads





C++ concurrency

- C++ concurrency is a fairly low-level implementation
- It is based on std::thread class
- An std::thread object maps onto a software thread managed by the operating system



Thread refers to sofrware threads and instances of the C++ std::thread class

A task will be a high-level abstraction, relating to some work that should be performed concurrently





At the application level concurrency can be either

- single-threaded each activity is performed by a separate executing program instance

  - Historically, each activity was performed as a single process
  - each process has its own execution path or thread through the program's code and its own private memory space
  - Concurrency is achieved by starting a new process for each activity. If an activity needs to exchange data or communicate with another activity, interprocess communication is required
  - Each process has its own private  memory space
  - processes can be run on different computers

- multi-threaded - all activities are performed inside a single executing program instance

  - A single process is used to perform all activities

  - Each activity has its own execution path or thread

  - Concurrency is achieved by starting a new thread for each activity

  - All the threads share a common memory space

    - Can access global data
    - pointers can be passed between threads
    
  - Threads are light weight processes
  
    - Less overhead for OS to set up
  
    - Smaller task switching overhead
    
    - Easy to share data communicate between threads
    
  - Lack of data protection between threads, required careful programming
    
    
    
     



## 3. Getting start with threads

### 3.1 Launching a Thread

Non-threaded C++ programs have a single thread

- This call main() function
- The main function calls other functions, which run in same thread
- When the main() function returns the thread ends
- The program ends when the main thread ends

<img src="/home/yinghanhuang/TUM/MA/Multithread/images/1.jpeg" style="zoom: 50%;" />

Multi-threaded C++ programs have a main thread

- The main thread can create additional threads
- Which in turn can create further threads
- Each thread has its own entry point function
- When the thread starts executing, its entry point function is called
- When this function returns, the thread ends
- The main thread continues its own execution path, it does not wait for the other threads unless we explicitly tell it to 

<img src="/home/yinghanhuang/TUM/MA/Multithread/images/2.jpeg" style="zoom:50%;" />







**Thread creation**

- To create a new execution thread, we need an instance of std::thread  
  - This is defined in <thread>
  - An std::thread instance can not be copied, but can be moved
- Its constructor takes a callable object as it first argument, this represents the entry point to the thread
- Thread function cannot be overloaded, can take arguments by value, by reference or by move, any return value will be ignored



**Thread execution**

The std::thread object starts running as soon as it is created, when the thread runs, it will execute the code in callable object

To make the parent stop and wait for the thread, we call its join() member function

- if we do not do this, the parent will carry on executing
- Thus the parent could terminate before the thread has run, which is not good



### 3.2 The C++ Thread class

Pausing thread

- we can pause a thread or make it "sleep" by calling std::this_thread::sleep_for()
- It takes an argument of type std::chrono::duration
- Which specifies the time the thread pauses for



Thread ID

- Each std::thread object has a thread identifier of type std::thread::id
  - This value is actually not important to programmer
  - Guaranteed to be unique: if 2 identifiers are equal, the objects are identical
  - A new thread may have the ID of a previous thread, if that thread has completed
- These identifiers can be used as keys to store thread objects in associative containers
- To obtain the ID of current thread
  - std::this_thread::get_id()
- To find the ID of an std::thread object, call its get_id() member function
  - t.get_id()





It is possible to get the underlying OS system of thread, using native_handle()

- The OS typically provide an API which consists of C functions which take a thread handle as the first argument
- std::thread has a native_handle() member function which will return the underlying software thread's handle
- This can be used to access functionality provided by the underlying implementation
- These provide functionality which is not available in standard C++
  - Thread priority - give a thread higher or lower share of processor time than others
  - Thread affinity - specify on which CPU or processor core a thread should run



Moving std::thread objects

- std::thread has ownership semantic s (like unique_ptr)
  - Only one thread object can be bound to an execution thread at a time
  - thread objects cannot be copied
  - However, they can be moved, this transfers ownership of the execution thread from one object to another

This can be used to passing std::thread objects to a function using pass by move

```c++
void f(thread t){...}                  // function taking a thread object as argument
```

Either using a temporary object

```c++
f(thread{func});                      // Pass temporary to f
```

Or using std::move

```c++
thread t{func};                      
f(move(t));                      // pass variable f using std::move()
```





Returning std::thread objects from functions

- we can also return them from functions

```c++
thread g()
{
	thread t{func};
	return t;
}
```





std::thread and exceptions

- Each execution thread has its own call stack, when there is an exception in this thread, other threads in the program, including the main thread, cannot catch the exception
-  When an exception is thrown, this stack is unwound in the normal way
  - The destructors for all objects in scope are called
  - **The program moves up the thread's stack until it finds a suitable handler**
  - If no handler is found, the program is terminated
- Exceptions can only be handled within the thread where they occur
  - Just use the try/catch function in normal way





### 3.3 Managing a thread

Detaching a thread

- Instead of calling join() on a thread, we can call detach()
  - This will cause the parent thread to continue executing
  - The detached task will run until it completes (or the program terminates)
  - This is analogous to a "daemon" process
  - If the detached thread is still running when the program ends, its destructor is called
  - The resource needed to join the thread are released immediately



<img src="/home/yinghanhuang/.config/Typora/typora-user-images/image-20221125141326770.png" alt="image-20221125141326770" style="zoom:50%;" />  





Exception thrown while managing a thread

- When an exception is thrown in the parent thread, the destructors are called for every object in scope
- The destructor for std::thread checks whether join() or detach() have been called on the object\
- If neither has been called, the destructor will call std::terminate(). the program terminate immediately, which is not we want
- so we must call either join() or detach() before the thread is destroyed

e.g

```c++
{
	std::thread t{...};
	while (do_something()){...}   // if execption occurs here,the destructor of the thread object will be called, because the 
	t.join();					// compiler can not see the .join(), this will abruptly end the program
}
```



solution

1. use Try/catch solution, this is verbose and not very elegant ( we have to call join twice)

   ```c++
   {
   	std::thread t{func};
   	try{
   		while (do_something()){...}    // Might throw an exception
   	}
       catch (std::exception& e){
           t.join();						// Call join() before thread destructor is called
       }
       t.join();							// No exception if we got here -call join() as usual
   }
   ```

   

2. RAII solution
   - A better solution is to use the RAII idiom
     - Wrap the thread object inside a class
     - The class's destructor will call join() on the thread object
   - A thread object can only be joined once
     - Calling join() twice is an error
   - We can call joinable() to check whether it is sage to call join
     - if join() or detach() have already been called on the thread object
     - Or the thread object does not have an associated execution thread
       - Returns false
     - Otherwise, it did need to call join
       - Returns true
   - Attention:   thread is a move-only object, we have to use as move constructor in the class
   - ==keyword explicit==: Specifies that a constructor or conversion function (since C++11)or [deduction guide](https://en.cppreference.com/w/cpp/language/ctad) (since C++17) is explicit, that is, it cannot be used for [implicit conversions](https://en.cppreference.com/w/cpp/language/implicit_cast) and [copy-initialization](https://en.cppreference.com/w/cpp/language/copy_initialization).

> thread_manage.cpp



3. C++ 20 solution
   - C++20 provides  std::jthread
   - jthread's destructor automatically call join() if needed
   - We do not need to write any extra code to make our program behave safely
   - No widely supported by compiler yet 



### 3.3 Multiple Threads

Thread scheduling

- A scheduler controls how thread execute
  - Allocates threads to multiple cores
  - Performs task switching between 
- The scheduler can interrupt a thread to allow another thread to run
  - Threads may start in any order
  - A thread may be interrupted at any time
  - A thread may be restarted at any time
  - While a thread is waiting, sleeping or interrupted, another thread may run
  - This could run the same code as the inactive thread or access data which it shares with the inactive thread





Data sharing between threads

- It is very easy to make data shareable by different threads

- The only requirement is that the data is visible to thread functions

  - Global or static variable with global thread functions
  - Class member with class member thread functions

- However, interleaved execution means that threads can interfere with each other while modifying shared data

  - Values written by one thread can be overwritten by another thread

  - A value can be partially written by one thread and partially written by another $\Rightarrow$ Problem 

    

Data races can occur when we have accesses from multiple threads which are not ordered or atomic

- Potentially conflicting accesses to the same memory location
  - Two or more threads access the memory location and at least one of threads modifies it
- These are safe only if the accesses are synchronized in some way
  - Each access is made to "happen before" the next
  - Or all accesses are made "atomically"
- Otherwise a data race occurs, undefined behavior, the program is not guaranteed to behave consistently





Example of data races

> Data_race.cpp











## 4 Working with shared data

The idea of critical section, this is a region of code that must execute isolation from the rest of the program



### 4.1 Mutex Introduction

Mutex: a tool that prevents more than one user accessing the resource at a time

The mutex is a data structure with 2 states, "locked" or "unlocked"



Mutex and threads:

- A thread can only access the resource if the related mutex is unlicked
- So that each thread access the resource happens before B,C
- If a thread modifies the resource, the new value will be seen by the other threads when they access it



C++ mutex

- The standard library defines a mutex class in <mutex>

- A mutex object must be defined outside any thread functions, but visible in them

  - For example global/static variable with global thread functions
  - class data member with member thread functions

- It has 3 main member functions

  - lock() tries to lock the mutex, blocks if not available

  - try_lock() tries to lock the mutex, return immediately if not available

  - unlock() releases the lock on mutex, usually called in a loop

    ```c++
    while(!the_mutex.try_lcok()）{// keep trying to get lock
    	// could not get the lock-do sth else and try again later
    	this_thread::sleep_for(100ms)
    }
    // Finally get lock, can now execute critical region
    ```

    

Acquire and release semantics

- Lock a mutex
  - Acquire exclusive access to the data it protects
- Unlock the mutex
  - Release exclusive access to the data it protects
  - Also releases the result of any modification made to that data
  - The next thread that lock the mutex will acquire this modification
- These acquire and release semantic impose an ordering on accesses to the variable
  - There is no data race and data is always in a consistent state

> try_lock.cpp
>
> Thread_mutex.cpp



### 4.2 Lock guard

std::lock_guard



if u lock in this thread, but there is an exception before unlock, then the programm will jump out of this scope and u left the lock with locked. All the thread waiting for the acquire a lock will blocked forever

main() is joined on these blocked thread and will be blocked forever as well

- solution: use try and catch and unlock the mutex in catch





Drawback of std::mutex

- calling lock() requires a corresponding call to unlock()
- The programmer must ensure that unlock() is always called after the critical region so that other threads can enter it
- If unlock() is not called, the mutex will remain locked after the thread ends

For these reasons we do not normally use std::mutex directly



Mutex wrapper classes

C++ standard library provides some classes in <mutex> which wrap std::mutex

These use the RAII (Resource Allocation is Initialization) idiom for managing resources

- in this case, the constructor takes a mutex object  as argument and locks it
- The deconstructor unlocks the mutex



If we create a wrapper class on the stack, its mutex will always be unlocked when the object goes out of scope.



One of the present **std::lock_guard**

This is a very basic wrapper which only has 2 member functions

- constructor which locks the mutex
- deconstructor which unlocks the mutex

std::lock_guard takes the type of the mutex as a template parameter

```c++
std::lock_guard<mutex>lk(print_mutex)
```

这样就不用实例化一个mutex再锁了

```c++
void task1(){
    std::cout << "task1 trying to get lock" << std::endl;
    std::lock_guard<mutex>lk(print_mutex)
    std::cout << "task1 has lock" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
}
```



std::scoped_lock (from C++17) works exactly the same with lock_guard, but it can lock more than 1 mutex at the same time

- in the constructor the mutex are locked in the order given in the constructor call
- In the constructor, the mutex are unlocked in the reserve order
- This avoids the possiblity of deadlock with multiple mutexes
- But when there is only one mutex, std::scoped_lock will likely to ignore it



### 4.3 Unique lock

std::unique_lock

- has the same basic features as std::lock_guard
  - Constructor locks mutex
  - Deconstructor unlocks mutex
- It has an unlock() member function
- We can call this after the critical region to avoid blocking other threads while we execute non-critical code

```c++
void task1(){
    std::cout << "task1 trying to get lock" << std::endl;
    std::unique_lock<mutex>uk(print_mutex)
    std::cout << "task1 has lock" << std::endl;
    uk.unlock();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
}
```

it is more flexible than lock_guard



std::unique_lock constructor arguments

- The constructor can take a second argument
  - std::defer_lock
    - Does not lock the mutex. It can be locked later by calling lock() on the unique_lock or by passing the unique_lock instance to std::lock()
  - std::adopt_lock takes ownership pf a mutex that is already locked 
    - Locking a mutex that is already locked causes undefined behavior
  - std::try_lock
    - call try_lock() on the mutex instead of lock()
    - Returns immediately if the mutex cannot be locked
    - can call owns_lock() later to check whether the mutex owns the lock





**Recommendations**

- Use lock_guard to lock a single mutex for an entire scope
- Use scoped_lock to lock more than one mutex for an entire scope
- Use unique_lock if you need to unlock within the scope
- Use unique_lock if you need the extra flexibility





### 4.4 Timeouts and Mutexes

Recursive_mutex

- if a thread tries to lock an std::mutex which it has already locked, this causes undefined behaviour
- This can be avoided by using std::recursive_mutex instead of std::mutex
- Its lock() member function can be called repeatedly
  - For each lock() call, there must be an unlock() call
- It works with lock_guard



Timed Mutexes  std::timed_mutex

Timed mutexes have member functions which do not lock the mutex immediately

- try_lock()
  - Return immediately if lock is not available
- try_lock_for()
  - Keep trying to lock for the specified duration
- try_lock_until()
  - Keep trying to obtain the lock until the specific time



Mutexes timeout caveats

The chrono library provides several "clocks" which can be used to obtain durations and time points

- chrono::system_clock
  - Get time from operating system
  - May change erratically due to leap seconds, system time changes
  - Use it fort time points
- chrono::steady_clock
  - Always increases steadily
  - Use it for intervals
- try_lock_for and try_lock_until may return later than requested, due to scheduling issues



### 4.5 shared mutex

just allow one thread to have access to the critical session may be too restrictive, for example single writer, multiple readers



Sometimes it is safe for many threads to access a resource concurrently, **provided that only one thread can modify it at a time**



std::shared_mutex

- std::shared_mutex is defined in <shared_mutex>

- it can be locked into 2 different ways

  1. Exclusive lock

     No other thread can lock the mutex until this thread releases the lock

     - lock_guard<shared_mutex>
     - unique_lock<shared_mutex>
     - scoped_lock<shared_mutex>

  2. Shared lock

     - Any other thread may acquire a shared lock on the mutex
     - No other thread can acquire an exclusive lock until all the threads with shared locks on the mutex release them

     shared_lock<shared_mutex>

```c++
std::shared_mutex sm;
void write(){
	lock_guard lk(sm);    // write thread with exclusive lock
}

void read()
{
	shared_lock lk(sm);  // read thread has shared lock
}
```



### 4.6 shared data initialization

shared data

- shared data can take the form of
  - Global variable or static variable at namespace scope
    - Potentially shared by all threads in the program
  - Class member which is declared static
    - Potentially shared by all threads which execute member function of the class
  - Local variable in a task function which is declared static
    - Shared by all threads which execute that function





Shared data initialization

- in all cases except for local variables, a global or static variable is initialized when the program starts up
- At that point, only one thread is running, so there is no data race

- static local variable initialization

  - A static local variable is initialized when the program flow passes through its declaration for the first time

    ```c++
    void func()   // Task function
    {
    	static string s{"xyz"};   // initialized by the first thread that executes this code 
    }
    ```

  - Before C++11, the behaviour was undefined

  - However, C++11 specifies that the initialization can only happen on one thread

    - Any other thread that reach that code is blocked until the first thread has finished initializing the variable
    - The initialization happen before any access by other thread
    - No data race



Singleton Class

- used to implement the Singleton design pattern
- A singleton class has only a single global instance
- Its copy and move operators are deleted, which prevent the program from creating new instances
- A global function returns the instance of the class
  - If the instance does not exist yet, it is created and initialized
  - Otherwise, the existing instance is returned



Classic singleton implementation

- Typical singleton implementation. In the presence of multiple threads there is a data race ( because the pointer "Single" would be multiple times accessed)

  ```c++
  class Singleton;
  Singleton *single = nullptr;    // global pointer to Singleton instance
  
  Singleton* get_singleton()		// global function to obtain Singleton instance
  {
  	if (single == nullptr)
  		single = new Singleton(...);
  	return single;
  }
  ```

- With the guarantees about static local initialization in c++11, writing a threadsafe function to return a Singleton is now trivial. We dont need a golbal pointer any more, just a static local variable

  ```c++
  class Singleton;
  
  Singleton& get_singleton()
  {
  	static Singleton single;
  	return single;
  }
  ```

  The first thread that calls this function will create the singleton instance

  Subsequent threads will get the instance created by the first thread



### 4.7 Thread-local data

C++ supports thread-local variables

we use the thread-local keyword to declare a variable as local to a thread

These behave rather like static variables

However, there is a separate instance for each thread, instead of single instance which is shared by all threads



- Thread-local variables can be

  - Declared at namespace scope
  - Static data members of a class
  - Local variables in a function

- Static and namespace scope thread-local variables are always

  constructed at or before the first use in a translation unit

- Local variables which are thread-local are initialized in the same way as static local variables

- Thread-local variables are destroyed when a thread function returns

  - Where the destructors are called in the reverse order of construction (but there is not specific constructor order)

> Thread_local_dara.cpp





### 4.8 Lazy initialization

The usual advice is to initialize variable at the point where they are declared, this is known as "eager" initialization

In lazy initialization, a variable is not initialized until the first it is used, this is mainly used when there is quite a bit of overhead involved in creating or initializing the variable.

It can be used in multi-thread code, but we need to avoid data races





One way to solve the problem of data race is to use std::call_once()



std::call_one() ensures that 

- A function is only called once
- It is done in one thread
- The thread cannot be interrupted until the function call completes



To use this, we create a global instance of std::once_flag, and pass this instace and a callable object to std::call_once()

```c++
#include <mutex>

class some_type{
	// ...
public:
	void do_it() {/* ... */}
};

some_type* ptr{nullptr}; // variable to be lazily initialized

std::once_flag ptr_flag;  // the flag stores synchronization data

void process()
{
	// pass a callable object which performs the initialization
	std::call_once(ptr_flag, [](){ptr = new some_type;});
	pre->do_it();
}
```

The initialization of ptr involves several operations, c++ perform these operation in following steps:

- Allocate enough memory to store a some_type object
- Store the address of the memory in ptr
- Construct a some_type object in this memory 

These can have data race when we use mutex between step2 and 3



After c++17 the initialization of ptr is conducted as

- Allocate enough memory to store a some_type object
- Construct a some_type object in this memory 
- Store the address of the memory in ptr

So there are now 4 ways to do threadsafe lazy initialization

- Naive use of a mutex
- Use std::call_once()
- Double checked locking with c++17 compiler
- Make a ptr static local variable, if it is not needed outside process()



### 4.9 Dead lock

A thread is deadlocked when it cannot run at all

- Deadlock often occurs when two or more threads are waiting for each other

  - Thread A waits for thread B to do something while B is waiting for A
  - Threads A and B are waiting for an event that can never happen

  



Deadlock avoidance guidelines

- The basic rule: never wait for a thread if there is a possibility it could wait for your thread
  - Deadlock is not restricted to 2 threads (cycle where thread A waits for thread B, B waits for C and C waits for A )
- Avoid nested locks
  - If your thread already holds a lock, don't acquire another one
  - If you need multiple locks, acquire them in a single operation
- Acquire locks in a fixed order
  - If u need multiple locks and can't acquire in a single operatiom, acquire them in the same order in every thread

- Avoid calling "unknown code" within a critical section ,unless u know it wont cause lock



A better solution is to use library features to lock both mutexes at the same time

- In C++ 17, we can use scoped_lock, the following code won't cause problem

  ```c++
  void func1()
  {
  	scoped_lock lk1(mutex1, mutex2);
  }
  
  void func2()
  {
  	scoped_lock lk1(mutex2, mutex1)
  }
  ```

  

- in c++11/14, we lock both the mutexes, then pass the std::adopt_lock option to unique_lock's constructor

  ```c++
  lock(mutex1, mutex2);   // Lock both mutexes
  unique_lock<mutex> lk1(mutex1, std::adopt_lock);   // Associate each mutex with a unique_lock
  unique_lock<mutex> lk2(mutex2, std::adopt_lock);
  ```

  Alternatively, we can pass the std::defer_lock option and lock both mutexes later

  ```c++
  unique_lock<mutex> lk1(mutex1, std::defer_lock);   // Associate each mutex with a unique_lock
  unique_lock<mutex> lk2(mutex2, std::defer_lock);
  lock(mutex1, mutex2);    // Lock both mutexes
  ```

  



### 4.10 Live lock

The program makes no progress, but the threads are still active

Livelock can result from bad deadlock avoidance

- instead of thread blocking immediately when it cannot get a lock, it backs off and tries again



We should choose a smarter deadlock avoidance

- Thread priority
  - Another way to avoid livelock is to assign different priorities to threads
  - This is not directly supported by C++, but most OS allow us to do this
    - When choosing which thread to run, the scheduler will give preference to a thread with high priority over a thread with low priority
    - A high priority thread will run more often
    - A low priority thread will sleep or be interrupted more often
    - Available via std::thread's native_handle()
- Priority problem
  - Priority inversion
    - For example high-priority thread needs some data from a low priority thread
    - So the high priority is forced to run at the speed of a low priority thread (behaves like a low priority)





Deadlock and livelock are both examples of resource starvation, which occurs when a thread can not get the resources it needs to run.





### 4.11 Summary

1. Dont lock for any longer than necessary
   1. Other threads waiting for the resource will have to wait longer, which affects performance
   2. In particular, avoid input/output operations as these are very slow
2. Typically we lock before accessing the  resource, then unlock before doing any processing, give other threads more oppotunities to run
   1. Often useful to make a private copy
   2. If we need to modify, we can lock again
3. Dont lock any more data elements than necessary
4. Dont make locking too fine-grained, lock in a single operation
5. When using a mutex to protect shared data, use classes instead of global data and functions
   1. Make the shared data a member of the class
   2. Make the mutex a member of the class
   3. Make the task function a member function
   4. Provide a getter function to retrieve the shared data







## 5. Thread Synchronization

### 5.1 Condition Variables

We don't want it to be a too often cases that one thread should wait longer for a data from another thread or it work only after another thread finished



we can set a signal, when one thread finished, the other thread can know

Possible implementation

- Use a shared  variable, protected by a mutex
  -  The fetching thread will set this variable to true when it finishes
  - The progress bar thread runs a loop which checks the variable
  - The processing thread runs a loop which check the variable
- This is not very efficient
  - The loops in the waiting thread will consume a lot of processing power
  - When the other threads are checking the flag, the fetching thread is blocked from setting it



Better solution: **condition variable**

- A condition variable coordinates 2 or more threads while allowing critical sections to be protected by mutexes
  - The processing thread tells the condition variable it is waiting
  - The fetching thread sends a notification to the condition variable when it finishes.
  - The condition variable wakes up the processing thread, which then runs
- Condition Variables scenario
  - The processing thread creates a unique_lock instance to lock the mutex
    - It calls wait() on the condition variable
    - The condition variable releases the lock and block this this thread
  - The fetching thread creates a lock_guard instance to acquire a lock
    - It fetches the data
    - Releases the lock
    - Notifies the condition variable
  - When the condition variable is notified by the fetching thread, the processing thread is woken up
    - It acquires a lock, resumes execution and processes the data



std::condition_variable

- std::condition_variable is defined in <condition_variable>
- wait() takes an argument of type unique_lock <std::mutex>
  - It unlocks its argument and blocks the waiting thread until a notification is received
- Timeout versions wait_for() and wait_until()
- notify_one() sends a notification to single waiting thread
  - Only one of the threads which called wait() will be woken up
  - The other waiting thread will remain blocked
  - A different read thread processes the data each time
- notify_all() sends a notification to all waiting thread
  - This cause all the threads which have called wait() on the condition variable to be woken up
  - The threads could be woken up in any order
  - All the reader threads process the data at the same time



> condition_variable.cpp





### 5.2 Condition Variables with Predicate

The examples before suffer from a data race.

By default wait() will block until a notification is received, if the writing thread notifies the condition variable before th reading thread calls wait(), the reading thread is blocked forever,        === **"lost wakeup"**



Occasionally, the waiting thread will be woken up even though the writing thread has not notified the condition variable  === **"false wakeup"**



==> Solution: wait() with predicate

- wait() takes a second argument, which is a predicate
  - This also applies to wait_for() and wait_until()
- Typically, this predicate function checks a shared bool
- The bool is initialized to false. It will be set to true when the writing thread modifies the data
- The waiting thread will only resume execution if the predicate function returns true



Using wait with predicate

- Add a shared boolean flag, initialized to false

- In the wait() call, provide a callable object that checks the flag

  ```c++
  bool condition {false};   //Flag
  
  void reader()
  {
  	unique_lock<std::mutex> lk(mut);
  	cv.wait(lk, []{return condition;});     // Lambda predicate funcrtions that checks the flag
  }
  ```

  with this , even the writing thread finish before reading thread starts, it can still receive the data



### 5.3 Futures

Transferring data between threads

- it is a pity that std::thread does not provide a way to return a value from a thread
  - we transfer data from one thread to another by using a shared variable, this need to be protected by locks, to avoid data races
- To accomplish that the C++ standard library provides std::future and std::promise
  - These can be used to transfer data between threads via "shared state"
  - No explicit locking or shared data variable is required





Futures and promises use a producer-consumer model

- Producer thread generates data
  - The promise object sets the value in the shared state
- Consumer thread waits for data
  - The future object waits until the value is available
- Consumer thread gets this value
  - The future object returns the value from shared state



Future

- A future represents a value that has not yet been computed
- std::future is one of the most important data structures in C++ concurrency
  - It works with many different asynchronous objects and operations, not just std::promise
- An std::future object is not usually created directly
  - Obtained from a std::promis![image-20221129103303702](/home/yinghanhuang/.config/Typora/typora-user-images/image-20221129103303702.png)e object, or some other asynchronous operation



std::future

- std::future is a template class defined in <future>
  - The parameter is the type of the value will be computed
- The get() member function obtains the value when ready
  - This block until the operation is complete
  - Then fetches its return value
- std::future also has member functions which wait
  - wait() blocks until the operation is complete
  - wait_for() and wait_until() block with a timeout



Using std::future with std::promise

- We obtain the future instance from the promise by calling promise's get_future() member function

  ```c++
  promsie<int> p;
  auto f{p.get_future()};
  ```

- When the consumer is ready to receive the result, it calls the future's get() member function





 ### 5.4 Promise

An std::promise object is associated with a std::future object

- Together, they create a "shared state"
  - The std::promise object sets a value in the shared state
  - The std::future object get the value from the shared state
- This is analogous to the producer thread returning the value to the consumer thread



std::promise is a template class

- the parameter is the type of the result

- get_future() member function
  - Returns the std::future object associated with this promise
- set_value()
  - Set the results to its argument
- set_exception()
  - indicates that an exception has occurred
  - This can be stored in the shared state





```c++
#include <future>
void produce(std::promise<int>& px){            // Producer function with promise
	int x{42};
	this_thread::sleep_for(500ms);
	px.set_value(x);							// Set the result
}

void consume(std::future<int>& fx)
{												// Consume function with future
	int x=fx.get();								// Get the result
	std::cout << "This answer is " << x << std::endl;
}

int main(){
    promise<int> p;								// create promise instance
    future<int> f=p.get_future();				// Get the associate future
    
    thread fut{consume, std::ref(f)};			// Start consumer thread with future
    thread prom{produce, std::ref(p)};			// Start producer thread with promise
    
    fut.join();
    prom.join();
}
```





### 5.5 Promises with multiple waiting Threads

with std::future, only one consumer thread can get the value from the producer

- std::future is designed to have exclusive access to the shared state
- future instances cannot be copied 
- If more than one thread calls get() on the same future, there is a data race



std::shared_future allows multiple consumers to wait for a result from a producer thread

- It can be copied, so each thread has it own instance
- Calling get() from different copies is safe





obtaining a shared_future instance

- As with std::future, we do not normally create a shared_future instance directly

we can move an existing future object into a shared_future

```c++
promise<int> p;
future<int> f=p.get_future();
shared_future<int> sf1{std::move(f)};
```

we can call share() on the future to get a shared_future from it

```c++
shared_future<int> sf1{f.share()};
```

we can also obtain a shared_future directly from the promise

```
shared_future<int> sf1{p.get_future()};
```





## 6. Atomic Types

### 6.1 Integer Operations and Threads

On x86, all integer operations are performed as a single instruction, this means that a thread cannot be interrupted while performing integer operations



> potential of data race: single memory location which is accessed by multiple threads with modification





![image-20221129102219631](/home/yinghanhuang/.config/Typora/typora-user-images/image-20221129102219631.png)

simple program model

- A program runs on a processor
- The processor has memory cells called registers
- These registers store data which is needed to execute the current instruction
- When a task switch occurs, the current thread's registers are saved and the new thread's registers are loaded up
- Data is loaded from RAM into processor registors as needed
  - If modified, it is then written back to RAM





we need to make sure that

- thread A is not interrupted until it has written the new value to RAM
- thread B uses the new value

with a mutex, this is automatically done when we call lock() and unlock()

Here we can do this by declaring count as "atomic". This will cause the compiler to generate instructions which synchronize the threads accessing count.

​	This prevents a data race

> Integer_ops_with_atomic.cpp

### 6.2 Atomic Types

C++ 11 defines an atomic template in the <atomic> header

The parameter must be a type which is "trivially copyable"

- The copy and move constructors do nothing, or defaulted
- Usually only bool, integer types and pointers are used



For more complex types, locks may be silently added

- in this case, use a pointer to the type (which wont be locked)



Using a std::atomic<T> object

- the object must be initialized

  ```c++
  atomic<int> x{0};
  ```

- We can assign to and from the object

  ```c++
  x = 2;
  y = x;
  ```

  

in c++ make a variable "volatile" has no effect on the behaviour of the multi-thread program



### 6.3 Atomic operation

  

static member funcion

- store()
  - Atomically replace the objects value with its argument
- load()
  - Atomically return the object's value
- operator =()
- operator T()
  - Synonyms for store() and load()
- exchange()
  - Atomically replace the object's value and return the previous value



specialized member functions

- pointer specializations support pointer arithmetic operations
  - Increment and decrement operators for pointers, += and -= operators
- Integer specializations have these and also
  - fetch_add()
    - equivalent to x++
  - fetch_sub()
    - equivalent to x--
  - Atomic bitwise logical operations



std::atomic_flag

- std::atomic_flag is an atomic boolean type, it is guaranteed to be lock-free
  - it is less overhead than std::atomic<bool>
- It only supports 3 operations
  - clear() set flag to false
  - test_and_set() sets flag to true and returns previous value
  - operator=()
- We must initialize an atomic_flag instance to false, where there is only one option
  - atomic_flag lock = ATOMIC_FLAG_INIT;



The idea is, u create a atomic_flag variable, which is initially false, then u have several threads which try to call test_and_set(), if the return value is true, then they means the flag already has the value true, we have not changed it; if the return value is false, then that means we have changed the value of flag

we can use the property to impose a "happen before" ordering on threads which are checking this flag

e.g we can use std::atomic_flag to implement a basic spin lock

- initialize the flag to false
- Each thread begins by calling test_and_set()
- if this returns true, some other thread is in the critical sections, loop and try again
- If it returns false, this thread has set the flag and it is safe to proceed
- Finally set the flag to false to allow another thread to enter the critical section



> atomic_spin_lock.cpp





pro and cons of spin lock

- A spinning thread remains active, so it can continue immediately when it "gets the lock"
  - With a mutex, the thread may need to be reloaded or woken up
- Processor-intensive- **Only suitable for protecting very short critical sections, and/or under very low contention**
  - Usually only used in operation systems and libraries
  - Performance can be heavily impacted if spinning threads interrupt each other
- Many os implement mutex as a "hybrid mutex"
  - Spin very briefly
  - If the thread has not been able to run, then put it to sleep





### 6.4 Lock-free Programming



Lock-free programming allow threads to execute critical sections without assistance from the operating system's locking facilities



This avoids or reduces some of the drawbacks to using locks

- Race conditions caused by forgetting to lock, or using the wrong mutex
- Risk of deadlock
- High overhead (mutex requires system call, it will stop the code running and waiting for the operating system's responce, the os will answer it and change the data memory, which modify kernel data structures)
- Lack of scalability causes by coarse-grained locking
- Code complexity and increased overhead caused by fine-grained locking
- Lack of composability ( functions which lock should not cause other functions which lock, even if tthe mutexes are different)



Advantage:

- threads can never block each other
- No possibility of deadlock or livelock
- If a thread is blocked, other threads can continue to execute
- Useful in real-time system
- Important in some situations
  - Double-check locking
    - **But atomic types do not support . or -> operators, we will have to copy ptr to a non-atomic pointer before we can use it, cast is not a good idea**
  - Reference counters
  - Spin locks



BUt : it can be very diifficult to write lock-free code which is correct and efficient, many data structure (like double-linked list) cannot be written as lock-free





## 7. Asynchronous Programming

### 7.1 Asynchronous programming



Operations can be performed synchronously and asynchronously

- synchronous
  - Wait for an operation to complete before starting the next one
- asynchronous
  - Start the next operation without waiting for the first one to complete



Blocking and multi-threading programs

- Blocking is undesirable in threaded programs

  - Blocking reduces throughput and responsiveness of blocked thread
  - Any thread which join with this thread also be blocked
- Particularly in critical section
  - Any threads which are waiting to execute the critical section will be blocked as well
  - Possibility of deadlock, if we are using locks
- Using asynchronous programming reduces the need to block
  - But may not necessarily avoid it completely



Asynchronous programming can be used to perform parallel operations



### 7.2 Packaged Task

std::packaged_task

- std::packaged_task provides a higher level of abstraction of using threads
- It associates a function with an std::promise
- An std::packaged_task represents an asynchronus operation
- It is a callable object
  - When it is called, the function is run and its return value is stored in the promise
  - We can then get the return value from the associated future



creating an std::packaged_task object

- std::packaged_task is a template class defined in <future>

  - The parameter is the signature of the function

  - eg. a function takes a string by const reference and double as input and returns an int, the sentence is like

    ```c++
    std::packaged_task<int(const std::string&, double)> pt;
    ```

  - The parameter of the associated promise is the return type of the function

- The constructor of std::packaged_task takes a callable object

  - This is the function that the associated std::promise will be passed to



Using an std::packaged_task object

- Unlike std::thread, a packaged_task does not start automatically
  - It can be run directly
  - Usually it is passed by move to an std::thread constructor so that it runs in its own thread
- std::packaged_task cannot be copied
  - It has ownership of its callable object and promise





Advantage of std::packaged_task

- using std::packaged_task avoids boilerplate code
- We can create a container of packaged_task objects and use it to start threads when we are ready for them
  - With std::thread, the thread starts running as soon as the object is created
- Useful for managing threads
  - Each task can be run on a specific thread
  - Thread scheduler runs thread in a certain order
  - Thread pool consists of threads waiting for work to arrive





> packaged_task.cpp





### 7.3 The async function

std::async()

- The std::async function is in <future>
- We can start a thread by calling std::async()
- async() represents a higher-level abstraction than std::thread
  - We can start a task with async and run it in the background
  - This allows us to do other work while the task is running
- async() uses similar syntax to std::thread's constructor
- We call it with the task function as the first argument, followed by the argument to task function

```
void hello()
{
	std::cout << "Hello Async\n";
}

int main()
{
	std::async(hello);   // create an instance of std::async and initialize it with the task
}
```





std::async() with std::future

- async() returns a future which contains the result of the asynchronous task
- when we are ready, we can call get() on the future to obtain the result
  - This allows us to use task functions which return a value
- If we only want to know that the background task has completed, we can wait() on the future
  - wait_for() and wait_until() if we want a timeout



Launch options

- async() may cause a new thread to be started to call the task function, or it may run the task in the same thread
- This is controlled by flags
  - std::launch::async
    - A new thread is started to call the task function
    - The task function is executed as soon as the thread starts
  - std::launch::deferred
    - this task is running in the same thread
    - the task would not be run until get() or wait() is called on the returned future
    - The task function is then executed ("lazy evaluation")
  - if both flags are set, the implementation decides whether to start a new thread, this is default
  - The launch flags can be set in optional extra first argument





### 7.4 choosing a thread object





## 8. Parallelism

### 8.1 Parallelism overview

Concurrency is different from parallelism, concurrency describes conceptually distinct tasks, these tasks are often interact, they may wait for an event, or wait for each other.



In parallelism, many instances of the same task run at the same time

- Run on multiple cores to improve scalability

These tasks mostly run independently of each other.



There are 2 ways to do parallelism

- Explicit parallelism
  - The programmer specifies how the work should be done in parallel
    - e.g divide data into 4 parts and start 4 threads to process each part
  - Involves more work for the programmer but can produce better performance
  - It is not scalable, if u have a computer with 4 cores before and u divide the program into 4 threads, then it work out, but if you have a new computer with 8 cores, it still run at 4 threads and not accelerating
  - Mainly useful when writing for specific hardware, or if the work naturally devides into a fixed number of ordered tasks
- Implicit parallelism
  - The programmer doesnt do anything, just leave the complier to specify how to do the work in parallel
  - Makes best use of available resources, usually the best option



### 8.2 Execution Policies



C++ 1998 introduced a new set of functions to the standard library

- These operate on containers and sequences of data
- They implement classic algorithms, such as searching and sorting
- Plus populating, copying, rendering etc

Most of them are in <algorithm>, a few are in <numeric>

An algorithm execution performs a series of operations on elements

- modification
- swap
- comparison
- function call application, etc





Code execution and parallelism

Modern hardware supports 4 different ways of executing code

- sequential
  - A single instruction processes a single data item, e.g. traditional loop
- vectorized
  - A single instruction processes several data items at the same time
  - Requires suitable data structure and hardware
- parallelized
  - Several instructions each process a single data item at the same time
  - Requires suitable algorithm
- parallelized + vectorized
  - Several instructions each process several data item at the same time
  - Requires suitable algorithm, data structure and hardware





Execution policy

- C++17 added "execution policies", included in <execution>, namspace std::execution
- These let us choose how a Standard algorithm call should be executed
  - seq - do not use parallel execution
  - par - use parallel execution
  - par_unseq - use parallel and vectorized execution
  - unseq - use vectorized execution (c++ 20)
- These are only requests, if the hardward/software not support, it may be ignored
- No every data structure can use execution policy, such as map

```c++
sort(v.begin(), v.end());      // Non-policy (sequential)
sort(seq, v.begin(), v.end());		// Sequential
sort(par, v.begin(), v.end());		// Parallel
sort(par_unseq, v.begin(), v.end()); // Parallel and vectorized		
sort(unseq, v.begin(), v.end());     // Vectorized
```





Sequenced execution

- With sequenced policy execution, all the operations in the algorithm execution are performed on a single thread, i.e. the thread which calls the algorithm

- Operations will not be interleaved, but may not necessarily be executed in a specific order

  ```c++
  vector<int> v(2000);
  int count{0};
  for_each(v.begin(), v.end(), [&](int& x){x = ++count;});
  for_each(seq, v.begin(), v.end(), [&](int& x){x = ++count;});
  ```

- In the second case the elements of v will have the same values as in the first case, but may not in the same order





Parallel execution

- The executions are performed in parallel across a number of threads, this may include the thread that called the algorithm function
- An operation will run on the  same operation for its entire duration, not jumping between threads
- Operations performed on the same thread will not be interleaved, but may not necessarily be executed in a specific order
- However, if no protection, there wil be data race





Unsequenced execution  (supported in c++20)

- The operations are performed on a single thread
  - i.e the thread which calls the algorithm, so we don't need to worry about data race
- operations will not be interleaved, but may not necessarily be executed in a specific order
- The programmer must avoid any modification of shared state between elements or between threads
  - Memory allocation and deallocation
  - Mutexes, locks and other forms of synchronization 



parallel unsequenced execution

- The executions are performed in parallel across a number of threads
  - This may include the thread that called the algorithm function
- An operation may be migrated from one thread to another
- Operations performed on the same thread may be interleaved and may not necessarily be executed in a specific order
- The programmer must avoid data races
- The programmer must avoid any modification of shared data between elements or between threads





### 8.3 Algorithms and execution policies

use instructions for execution policies: do not use execution policy



### 8.4 New Parallel Algorithm

std::accumulate()

- std::accumulate() takes a range of iterators and an initial value, it will add the value of each element in the range to that initial value and return the sum
- By default, the opeator + is used to perform the addition
- To perform a different operation, we can pass a callable object as an additional fourth argument
- std::accumulate() is specified to execute sequentially, not parallelized or vectorized 



std::reduce

- is a version of std：：accumulate() which supports all execution policies
- the operator of reduce must be commutative and associative





std::inclusive_scan

- std::partial_sum calculates the sum of the elements so far, the calculation in a fixed order

  ```c++
  std::vector<int> v{1,2,3,4};
  std::vector<int> v2(v.size());
  partial_sum(v.begin(), v.end(), v2.begin());
  ```

- std::inclusive_scan works the same way as std::partial_sum, but we can use execution policy here

- std::exclusive_scan calculates the sum of the elements so far, not including the current element, it takes an extra argument, which is used as if it were the first element of the input vector

  ```
  exclusive_scan(v.begin(), v.end(), v2.begin(), -1)
  ```





## 10. Practical Data Structures for Concurrent Programming

std::queue

- FIFO data structure
  - Elements are pushed onto the back of queue and popped off the front
- Removing an element involves 2 operations
  - front() returns a reference to the elemetn at the front
  - pop() removes the element at the front, without returning anything
  - if pop is called in empty containers, the behavior would be undefined
- is not suitable for use as a concurrent queue
  - Data race if the same instance is accessed from multiple threads
  - Race condition between front() and pop()
  - Undefined behaviour if we pop() an empty queue
- The simplest way to implement a concurrent queue is to write a wrapper class
  - std::queue instance is class member
  - std::mutex is class member
  - each member function locks the mutex, then calls the corresponding member function in the std::queue






concurrent queue member functions

- rule of five
  - Nothing special required. The defaults are sufficient
- push()
  - Locks the mutex, calls the std::queue's push() with its argument, then unlock the mutex
- pop()
  - Locks the mutex, calls the std::queue's front() and copies the returned value into argument
  - Then calls the std::queue's pop() and unlocks the mutex
  - Does sth. sensible if called on an empty queue



![image-20221130152712403](/home/yinghanhuang/.config/Typora/typora-user-images/image-20221130152712403.png)

![image-20221130152642296](/home/yinghanhuang/.config/Typora/typora-user-images/image-20221130152642296.png)



cpp:

![image-20221130153028765](/home/yinghanhuang/.config/Typora/typora-user-images/image-20221130153028765.png)

![image-20221130153054288](/home/yinghanhuang/.config/Typora/typora-user-images/image-20221130153054288.png)





### 10.2 Thread Pools

Creating a thread involves a lot of overhead

- create an execution stack for the thread
- Call a system API
- The operating system must create the internal data to manage the thread
- The scheduler must execute the thread
- A context switch occurs to run the thread

it can take 10000 times as long as the calling a function directly







Thread pool

Thread pool is a fixed-size container of thread objects

- usually equal to the number of cores on the machine
- This can be found by calling std::thread::hardware_concurrency()

it needs a queue of task function objects

- A thread object takes a task off the queue
- it will calls the task function
- when finished, it takes the next task from the queue





Overview

- the thread_pool class will contain
  - A vector of thread objects
  - A concurrent queue to store incoming tasks
- The user of class will call its submit() member function with a task function as argument 
  - This task function will be pushed on the queue
- Each thread runs in an infinite loop which calls pop() on the queue
  - When the pop() call completes, it will return a task function
  - The thread will then execute the returned task function

  
