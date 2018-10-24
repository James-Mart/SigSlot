#include <functional>
#include <iostream>
#include "SigSlot/SigSlot.h"
#include "SigSlot/Driver/Utilities/Timer.h"

class SomeObject : public SigSlot::SignalValueChanged<int>
                 , public SigSlot::SignalFilterValue<int&>
{
public:
    SomeObject() : m_number(0) { /*NOP*/ }
    virtual ~SomeObject() {}

    void Modify2(int p_newNumber)
    {
        //bool bValidOperation = EmitSignal_Test("AllowedToModify");
        //if (bValidOperation)
        {
            EmitSignal_FilterValue(p_newNumber);
            m_number = p_newNumber;
            EmitSignal_ValueChanged(p_newNumber);
        }
    }

private:
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

enum class LoginLevel { Developer, Administrator, Operator};
LoginLevel _UserPrivileges()
{   // In a real application this could query some user management interface
    return LoginLevel::Administrator;
}

int main()
{
    SigSlotUtils::Timer timer([](float p_time) {std::cout << "Function took " << p_time << "ms\n"; });

    SomeProcess process;
    SomeObject obj;

    //std::function<LoginLevel()> _userPrivs = _UserPrivileges;
    //obj.OnSignal_Test("AllowedToModify", _userPrivs, LoginLevel::Administrator);
    std::function<void(int)> _alert = [](int i) {std::cout << "Data changed to " << i << '\n'; };
    obj.OnSignal_ValueChanged(_alert);
    std::function<void(int&)> _filter = [](int& i) { i *=2; };
    std::function<void(int&)> _filter2 = [](int& i) { i /= 2; };
    obj.OnSignal_FilterValue(_filter);
    obj.OnSignal_FilterValue(_filter2);

    process.AddObject(obj);
    process.Run();


    return 0;
}
