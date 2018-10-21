#pragma once

#include <functional>
#include <vector>

namespace SlotExecution
{
    template<typename ...Ts>
    void ExecuteSlots(const std::vector<std::function<void(Ts...)>> p_funcs, Ts... p_args)
    {
        for (const auto& func : p_funcs)
        {
            func(p_args...);
        }
    }
};
