#pragma once
#include <chrono>
#include <functional>

namespace SigSlotUtils
{
    struct Timer
    {
        Timer(std::function<void(float)> p_GiveExecutionTime)
            : m_GiveExecutionTime(p_GiveExecutionTime)
            , m_start(std::chrono::high_resolution_clock::now()) 
        {
            // NOP
        }
        ~Timer()
        {
            m_duration = (std::chrono::high_resolution_clock::now() - m_start);
            m_GiveExecutionTime(m_duration.count() * 1000.0f);
        }

        std::chrono::time_point<std::chrono::steady_clock> m_start;
        std::chrono::duration<float> m_duration;
        std::function<void(float)> m_GiveExecutionTime;
    };
};