#include <iostream>
#include <thread>

class thread_guard{
    std::thread t;
public:
    explicit thread_guard(std::thread&& t) : t(std::move(t)) {}
    ~thread_guard(){
        if (t.joinable()) t.join(); // join thread if necessary
    }

    // Delete the copy operator
    thread_guard(const thread_guard&) = delete;
    thread_guard& operator=(const thread_guard&) = delete;

    // Provide move operators
    thread_guard(thread_guard&&) noexcept = default;
    thread_guard& operator=(thread_guard&&) noexcept = default;
};

int main()
{
    std::thread t{};
    thread_guard tg(std::move(t));
    return 0;
}