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

    namespace Actionables
    {
        class CanEmit
        {
        public:
            virtual ~CanEmit() { /* NOP */ }
            void EmitSignal(std::string p_signalId)
            {
                auto slotsIter = m_vecFailableSlots.find(p_signalId);
                if (slotsIter != m_vecFailableSlots.end()) 
                {
                    const auto& slots = slotsIter->second;
                    SignalEmitted(p_signalId, (slots.size() != Executor::ExecuteUntilFail(slots)));
                }
            }

            template <typename T, typename... Us>
            void OnSignal_CheckCriteria(std::string p_signalId, const std::function<T(Us...)>& p_criteriaFunction, const T& p_expectedResult, Us... p_args)
            {
                m_vecFailableSlots[p_signalId].push_back([&]() -> bool
                {
                    return (p_criteriaFunction(p_args...) == p_expectedResult);
                });
            }


        protected:
            virtual void SignalEmitted(std::string p_signalId, bool p_criteriaPassed)
            {
                // NOP - Override in child class to respond to passed/failed criteria checks
            }

        private:
            std::unordered_map<std::string, std::vector<std::function<bool()>>> m_vecFailableSlots;
            
        };
    };
};