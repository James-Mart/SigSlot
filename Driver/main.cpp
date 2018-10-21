#include <functional>
#include <iostream>
#include "SigSlot/SigSlot.h"
#include "SigSlot/Driver/Utilities/Timer.h"

int main()
{
    SigSlotUtils::Timer timer([](float p_time) {std::cout << "Function timer took " << p_time << "ms\n"; });

    std::function<void(int, int)> funcPrint = [](int p1, int p2) {std::cout << "Arguments: " << p1 << " " << p2 << '\n'; };
    std::function<void(int, int)> funcSum = [](int p1, int p2) {std::cout << "Sum: " << p1 + p2 << '\n'; };
    std::vector<std::function<void(int, int)>> funcs = {funcPrint, funcSum};
 
    SlotExecution::ExecuteSlots(funcs, 1, 2);

    return 0;
}