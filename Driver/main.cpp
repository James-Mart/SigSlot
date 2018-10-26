#include <functional>
#include <iostream>
#include "SigSlot/SigSlot.h"
#include "SigSlot/Driver/Utilities/Timer.h"

//Objects must specify what signal behavior they require
class SomeObject : public SigSlot::SignalValueChanged<int>
                 , public SigSlot::SignalFilterValue<int&>
{
public:
    SomeObject() : m_number(-1) { /*NOP*/ }
    virtual ~SomeObject() {}

    void Modify(int p_newNumber) {
        //Objects that use this pattern should emit signals where appropriate
        EmitSignal_FilterValue(p_newNumber);
        if (p_newNumber != m_number) {
            m_number = p_newNumber;
            EmitSignal_ValueChanged(m_number);
        }
    }

private:
    int m_number;
};

// An example process which uses some objects that emit signals
class SomeProcess
{
public:
    void AddObject(const SomeObject& obj) {
        m_objects.push_back(obj);
    }

    void Run() {
        // Other processes may use objects as they normally would
        for (auto& object : m_objects) {
            for (int i = 0; i < 11; ++i) {
                object.Modify(i);
            }
        }
    }

private:
    std::vector<SomeObject> m_objects;
};

int main() {
    JmUtils::Timer timer([](float p_time) {std::cout << "Function took " << p_time << "ms\n"; });

    SomeObject obj1;
    obj1.OnSignal_ValueChanged([](const int& i) {std::cout << "Object 1 data: " << i << '\n'; });
    obj1.OnSignal_FilterValue([](int& i) { i *= 2; }); //  0 -> 20
    obj1.OnSignal_FilterValue([](int& i) { i -= 1; }); // -1 -> 19

    SomeObject obj2;
    obj2.OnSignal_ValueChanged([](const int& i) {std::cout << "Object 2 data: " << i << '\n'; });
    obj2.OnSignal_FilterValue([](int& i) { i = 0; }); //  Always 0

    SomeProcess process;
    process.AddObject(obj1);
    process.AddObject(obj2);
    process.Run();

    return 0;
}
