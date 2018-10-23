#pragma once

#include <functional>
#include <vector>
#include <unordered_map>

namespace SigSlot
{
    namespace Executor
    {
        template<typename ...Ts>
        void Execute(const std::vector<std::function<void(Ts...)>>& p_slots, Ts... p_args)
        {
            for (const auto& func : p_slots)
            {
                func(p_args...);
            }
        }

        template<typename ...Ts>
        int ExecuteUntilFail(const std::vector<std::function<bool(Ts...)>>& p_slots, Ts... p_args)
        {
            int iCount = 0;
            for (const auto& func : p_slots)
            {
                if (!func(p_args...)) break;
                ++iCount;
            }
            return iCount;
        }
    };

    class CanAlert
    {
    public:
        virtual ~CanAlert() { /* NOP */ }

        void EmitSignal_Alert(const std::string& p_signalId)
        {
            auto slotsIter = m_vecSlots_Alerts.find(p_signalId);
            if (slotsIter != m_vecSlots_Alerts.end())
            {
                Executor::Execute(slotsIter->second);
            }
        }

        template <typename... Ts>
        void OnSignal_Alert(const std::string& p_signalId, const std::function<void(Ts...)>& p_alertFunction, Ts... p_args)
        {
            m_vecSlots_Alerts[p_signalId].push_back([&]() {
                p_alertFunction(p_args...);
            });
        }

    private:
        std::unordered_map<std::string, std::vector<std::function<void()>>> m_vecSlots_Alerts;
    };

    class CanTest
    {
    public:
        virtual ~CanTest() { /* NOP */ }

        bool EmitSignal_Test(const std::string& p_signalId)
        {
            auto slotsIter = m_vecSlots_Tests.find(p_signalId);
            if (slotsIter != m_vecSlots_Tests.end())
            {
                const auto& slots = slotsIter->second;
                return (slots.size() != Executor::ExecuteUntilFail(slots));
            }
        }

        template <typename T, typename... Us>
        void OnSignal_Test(const std::string& p_signalId, const std::function<T(Us...)>& p_criteriaFunction, const T& p_expectedResult, Us... p_args)
        {
            m_vecSlots_Tests[p_signalId].push_back([&]() -> bool {
                return (p_criteriaFunction(p_args...) == p_expectedResult);
            });
            
        }

    private:
        std::unordered_map<std::string, std::vector<std::function<bool()>>> m_vecSlots_Tests;
    };

    class CanFilter
    {
        /* Open design questions:
        *   1) How should we prioritize filters? Run in the order they are added? Provide a prioritization system?
        *      For now, I will just run in registration order.
        *   2) When a value is filtered, do we stop processing the remaining filters? Do we pass the filtered value
        *      to the remaining filters?
        *      For now, I will stop once a single filter changes the 
        */

    public:
        virtual ~CanFilter() { /* NOP */ }

        template <typename T>
        void EmitSignal_Filter(const std::string& p_signalId)
        {
            auto slotsIter = m_vecSlots_Filters.find(p_signalId);
            if (slotsIter != m_vecSlots_Filters.end())
            {
                const auto& slots = slotsIter->second;
                return (slots.size() != Executor::ExecuteUntilFail(slots));
            }
        }

        template <typename T, typename... Us>
        void OnSignal_Filter(const std::string& p_signalId, const std::function<T(Us...)>& p_filterFunction, Us... p_args)
        {
            m_vecSlots_Filters[p_signalId].push_back([&]() -> T {
                return p_filterFunction(p_args...);
            });
        }

    private:
        //Todo: Make this a variadic class template to allow many filterable types
        std::unordered_map<std::string, std::vector<std::function<int()>>> m_vecSlots_Filters; 
    };
};