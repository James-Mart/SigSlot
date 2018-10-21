#pragma once

#include <functional>
#include <vector>

namespace SlotExecution
{
    template<typename ...Ts>
    void ExecuteSlots(const std::vector<std::function<void(Ts...)>>& p_slots, Ts... p_args)
    {
        for (const auto& func : p_slots)
        {
            func(p_args...);
        }
    }

    template<typename ...Ts>
    int ExecuteSlotsUntilFail(const std::vector<std::function<bool(Ts...)>>& p_slots, Ts... p_args)
    {
        int iLastNr = 0;
        for (const auto& func : p_slots)
        {
            if (!func(p_args...)) break;
            ++iLastNr;
        }
        return iLastNr;
    }
};
