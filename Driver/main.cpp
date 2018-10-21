#include <functional>
#include <iostream>
#include <chrono>
#include "SigSlot/SigSlot.h"

int main()
{
    auto start = std::chrono::high_resolution_clock::now();

    std::function<void(int, int)> func = [](int p1, int p2) {std::cout << p1 << " " << p2 << '\n'; };
    std::vector<std::function<void(int, int)>> funcs = {func};
 
    SlotExecution::ExecuteSlots(funcs, 1, 2);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> duration = end - start;
    std::cout << "Function took " << duration.count() * 1000 << "ms\n";

    return 0;
}