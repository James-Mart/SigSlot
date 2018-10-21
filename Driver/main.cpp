#include <functional>
#include <iostream>
#include <chrono>
#include "SigSlot/SigSlot.h"

struct Timer
{
    Timer()
    {
        start = std::chrono::high_resolution_clock::now();
    }
    ~Timer()
    {
        duration = (std::chrono::high_resolution_clock::now() - start);
        std::cout << "Function timer took " << duration.count() * 1000.0f << "ms\n";
    }

    std::chrono::time_point<std::chrono::steady_clock> start;
    std::chrono::duration<float> duration;
};

int main()
{
    Timer timer;

    std::function<void(int, int)> func = [](int p1, int p2) {std::cout << p1 << " " << p2 << '\n'; };
    std::vector<std::function<void(int, int)>> funcs = {func};
 
    SlotExecution::ExecuteSlots(funcs, 1, 2);

    return 0;
}