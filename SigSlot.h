#pragma once

#include <functional>
#include <vector>
#include <unordered_map>

namespace SigSlot
{
    template <typename T, typename... Us>
    class SignalType
    {
    public:
        virtual ~SignalType() { /* NOP */ }

    protected: 

        void OnSignal(const std::function<T(Us...)>& p_alertFunction)
        {
            m_vecSlots.push_back(p_alertFunction);
        }

        void Emit(Us... p_args)
        {
            for (const auto& func : m_vecSlots)
            {
                func(p_args...);
            }
        }

        std::vector<std::function<T(Us...)>> m_vecSlots;
    };

    template <typename T>
    class SignalValueChanged : public SignalType<void, T>
    {
    public:
        virtual ~SignalValueChanged() { /* NOP */ }

        void OnSignal_ValueChanged(const std::function<void(T)>& p_slotFunc)
        {
            SignalType<void, T>::OnSignal(p_slotFunc);
        }

        void EmitSignal_ValueChanged(T p_newValue) 
        {
            SignalType<void, T>::Emit(p_newValue);
        }
    };

    template<typename... T>
    class SignalFilterValue : SignalType<void, T...>
    {
    public:
        virtual ~SignalFilterValue() { /*NOP*/ }

        void OnSignal_FilterValue(const std::function<void(T...)>& p_slotFunc)
        {
            SignalType<void, T...>::OnSignal(p_slotFunc);
        }

        void EmitSignal_FilterValue(T... p_value)
        {
            SignalType<void, T...>::Emit(p_value...);
        }
    };
};