#pragma once

#include <functional>
#include <vector>
#include <unordered_map>

namespace SigSlot
{
    /*****************************************************************
    * SignalType provides the basic signal interface
    ******************************************************************/
    template <typename... Us>
    class SignalType
    {
    public:
        virtual ~SignalType() { /* NOP */ }

        void OnSignal(const std::function<void(Us...)>& p_alertFunction) {
            m_vecSlots.push_back(p_alertFunction);
        }

        template <typename T>
        void OnSpecificSignal(T p_slotID, const std::function<void()>& p_slotFunction)
        {
            m_vecSlots.emplace_back([=](T p_ID) {
                if (p_slotID == p_ID) p_slotFunction();
            });
        }

    protected: 
        void Emit(Us... p_args) {
            for (const auto& func : m_vecSlots) {
                func(p_args...);
            }
        }

    private:
        std::vector<std::function<void(Us...)>> m_vecSlots;
    };

    /*****************************************************************
    * Here are some common example SignalType variants 
    ******************************************************************/
    template <typename T>
    class SignalValueChanged : public SignalType<T>
    {
    public:
        virtual ~SignalValueChanged() { /* NOP */ }

        void OnSignal_ValueChanged(const std::function<void(const T&)>& p_slotFunc) {
            SignalType<T>::OnSignal(p_slotFunc);
        }

    protected:
        void EmitSignal_ValueChanged(const T& p_newValue) {
            SignalType<T>::Emit(p_newValue);
        }
    };

    template<typename... T>
    class SignalFilterValue : SignalType<T...>
    {
    public:
        virtual ~SignalFilterValue() { /*NOP*/ }

        void OnSignal_FilterValue(const std::function<void(T...)>& p_slotFunc) {
            SignalType<T...>::OnSignal(p_slotFunc);
        }

    protected:
        void EmitSignal_FilterValue(T... p_value) {
            SignalType<T...>::Emit(p_value...);
        }
    };

    template<typename T>
    class SignalAlertEvent : SignalType<T>
    {
    public:
        virtual ~SignalAlertEvent() { /*NOP*/ }

        void OnSignal_AlertEvent(T p_event, const std::function<void()>& p_slotFunc) {
            SignalType<T>::template OnSpecificSignal<T>(p_event, p_slotFunc);
        }

    protected:
        void EmitSignal_AlertEvent(T p_event) {
            SignalType<T>::Emit(p_event);
        }
    };

    /*****************************************************************
    * Subclass "SignalType" whenever a new signal variant is required
    ******************************************************************/
};