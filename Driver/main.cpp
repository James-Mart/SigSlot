#include <functional>
#include <iostream>
#include "SigSlot/SigSlot.h"
#include "SigSlot/Driver/Utilities/Timer.h"

class SomeObject : public SigSlot::CanAlert
                 , public SigSlot::CanFilter
                 , public SigSlot::CanTest
{
public:
    SomeObject() : m_number(0) 
    {
        std::function<bool()> validationFunction = std::bind(&SomeObject::_IsValidData, this);
        OnSignal_Test("DataValid", validationFunction, true);
    }
    virtual ~SomeObject() {}

    /*void Modify(int p_newNumber) { 
        EmitSignal_Filter("FilterNumberChanging", p_newNumber);
        if (m_number != p_newNumber)
        {
            m_number = p_newNumber;
            EmitSignal_Alert("AlertNumberChanged");
        }
    }*/

    void Modify2(int p_newNumber)
    {
        bool bValid = EmitSignal_Test("DataValid");
        if (bValid)
        {
            m_number = p_newNumber;
            EmitSignal_Alert("AlertNumberChanged");
        }
    }

private:
    inline bool _IsValidData() const {
        return true;
    }
    int m_number;
};

class SomeProcess
{
public:
    void AddObject(const SomeObject& obj)
    {
        m_objects.push_back(obj);
    }
    void Run()
    {
        for (auto& object : m_objects)
        {
            for (int i = 0; i < 100; ++i)
            {
                object.Modify2(i);
            }
        }
    }

private:
    std::vector<SomeObject> m_objects;
};

void TestSlotFunctions() 
{
    // Run regular slot functions
    std::function<void(int, int)> funcPrint = [](int p1, int p2) {std::cout << "Arguments: " << p1 << " " << p2 << '\n'; };
    std::function<void(int, int)> funcSum = [](int p1, int p2) {std::cout << "Sum: " << p1 + p2 << '\n'; };
    std::vector<std::function<void(int, int)>> funcs = { funcPrint, funcSum };
    SigSlot::Executor::Execute(funcs, 1, 2);

    // Run failable slot functions
    std::function<bool(int)> funcPrintInt = [](int p1) {std::cout << "Int: " << p1 << '\n'; return true; };
    std::function<bool(int)> funcReturnFalse = [](int p1) {return false; };
    std::vector<std::function<bool(int)>> failableFuncs = { funcPrintInt, funcReturnFalse };
    int iNrSucceeded = SigSlot::Executor::ExecuteUntilFail(failableFuncs, 3);
    if (iNrSucceeded < static_cast<int>(failableFuncs.size()))
        std::cout << "Only " << iNrSucceeded << " out of " << failableFuncs.size() << " failable functions succeeded.\n";
}

int main()
{
    SigSlotUtils::Timer timer([](float p_time) {std::cout << "Function took " << p_time << "ms\n"; });

    //TestSlotFunctions();

    SomeProcess process;
    SomeObject obj;    
    process.AddObject(obj);
    process.Run();


    return 0;
}