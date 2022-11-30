#include <numeric>
#include <iostream>
#include <vector>
#include <execution>

int main()
{
    std::vector<int> v{1, 2, 3, 4, 5, 6, 7, 8};
    auto sum = std::accumulate(v.begin(), v.end(), 0);
    std::cout << sum << std::endl;
}
// g++ -pthread -std=c++1z new_parallel_algorithm.cpp -ltbb 