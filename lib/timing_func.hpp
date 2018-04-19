#pragma once
#include <chrono>
#include <iostream>
#include <algorithm>

template<typename TimeT = std::chrono::milliseconds> struct measure
{
    template<typename Func, typename ...Args> static typename TimeT::rep execution(Func &&func, Args&&... args)
    {
        auto start = std::chrono::steady_clock::now();
        std::forward<decltype(func)>(func)(std::forward<Args>(args)...);
        auto duration = std::chrono::duration_cast<TimeT> (std::chrono::steady_clock::now() - start);
        return duration.count();
    }
};


template<typename TimeT = std::chrono::milliseconds> struct compare_function
{
    template<typename Func1, typename Func2> static void exec(Func1 &&func1, Func2 &&func2)
    {
        auto start = std::chrono::steady_clock::now();
        std::forward<decltype(func1)>(func1)();
        double d1 = (double)std::chrono::duration_cast<TimeT> (std::chrono::steady_clock::now() - start).count();
        start = std::chrono::steady_clock::now();
        std::forward<decltype(func2)>(func2)();
        double d2 = (double)std::chrono::duration_cast<TimeT> (std::chrono::steady_clock::now() - start).count();
        std::cout << "Time execution 1st function : " << d1 << std::endl;
        std::cout << "Time execution 2nd function : " << d2 << std::endl;
        std::cout << "Speedup = " << ((d1 == 0.) ? 0. : d1/d2) << "X" << std::endl;
        return;
    }
};

template<typename T> static inline void check(T *a, const int &na, T *b)
{
    std::cout << "check array : " << ( (std::equal(a, a + na, b)) ? "GOOD!" : "BAD..." ) << std::endl;
    return;
}
