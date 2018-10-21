#include <functional>
#include <iostream>
#include "SigSlot/SigSlot.h"
#include "SigSlot/Driver/Utilities/Timer.h"

int main()
{
    SigSlotUtils::Timer timer([](float p_time) {std::cout << "Function took " << p_time << "ms\n"; });

    // Run regular slot functions
    std::function<void(int, int)> funcPrint = [](int p1, int p2) {std::cout << "Arguments: " << p1 << " " << p2 << '\n'; };
    std::function<void(int, int)> funcSum = [](int p1, int p2) {std::cout << "Sum: " << p1 + p2 << '\n'; };
    std::vector<std::function<void(int, int)>> funcs = {funcPrint, funcSum};
    SlotExecution::ExecuteSlots(funcs, 1, 2);

    // Run failable slot functions
    std::function<bool(int)> funcPrintInt    = [](int p1) {std::cout << "Int: " << p1 << '\n'; return true; };
    std::function<bool(int)> funcReturnFalse = [](int p1) {return false; };
    std::vector<std::function<bool(int)>> failableFuncs = { funcPrintInt, funcReturnFalse };
    int iNrSucceeded = SlotExecution::ExecuteSlotsUntilFail(failableFuncs, 3);
    if (iNrSucceeded < static_cast<int>(failableFuncs.size()))
        std::cout << "Only " << iNrSucceeded << " out of " << failableFuncs.size() << " failable functions succeeded.\n";



    return 0;
}