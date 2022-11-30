#include <algorithm>
#include <vector>
#include <iostream>
#include <execution>
#include <atomic>
// int main()
// {
//     std::vector<int> v(2000);
//     int count{0};
//     // std::for_each(std::execution::seq, v.begin(), v.end(), [&](int& x) {x = ++count;});
//     std::for_each(std::execution::par, v.begin(), v.end(), [&](int& x) {x = ++count;});
//     for (auto i:v)
//     {
//         std::cout << i << ",";
//     }
//     return 0;
// }

int main()
{
    std::vector<int> v(2000);
    std::atomic<int> count{0};  // use atomic to protect
    // std::for_each(std::execution::seq, v.begin(), v.end(), [&](int& x) {x = ++count;});
    std::for_each(std::execution::par, v.begin(), v.end(), [&](int& x) {x = ++count;});
    for (auto i:v)
    {
        std::cout << i << ",";
    }
    return 0;
}
// to run it g++ -pthread -std=c++1z execuation_policies.cpp -ltbb 