#include <functional>
#include <iostream>
#include <initializer_list>
#include <memory>

#include "SigSlot/SigSlot.h"
#include "SigSlot/Driver/Utilities/Timer.h"

enum class ObjectAEvents {
    Event_beforeExecute,
    Event_afterExecute
};

class ObjectA : public SigSlot::SignalAlertEvent<ObjectAEvents>
{
public:
    ObjectA() {}
    virtual ~ObjectA() {}

    void Execute()
    {
        EmitSignal_AlertEvent(ObjectAEvents::Event_beforeExecute);
        std::cout << "Object A execution!\n";
        EmitSignal_AlertEvent(ObjectAEvents::Event_afterExecute);
    }
};

//Objects must specify what signal behavior they require
class ObjectB : public SigSlot::SignalValueChanged<int>
              , public SigSlot::SignalFilterValue<int&>
{
public:
    ObjectB() : m_number(-1) { /*NOP*/ }
    virtual ~ObjectB() {}

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
    SomeProcess(std::initializer_list<std::shared_ptr<ObjectB>> objects)
    {
        for (auto o : objects) m_objects.emplace_back(o);
    }

    void Run() {
        // Other processes may use objects as they normally would
        for (auto o : m_objects) {
            for (int i = 0; i < 11; ++i) {
                o->Modify(i);
            }
        }
    }

private:
    std::vector<std::shared_ptr<ObjectB>> m_objects;
};

int main() {
    JmUtils::Timer timer([](float p_time) {std::cout << "Function took " << p_time << "ms\n"; });

    std::cout << "------- BEGIN: event emission test --------\n";
    ObjectA eventEmitter;
    eventEmitter.Execute();
    std::cout << "Attaching pre/post events...\n";
    eventEmitter.OnSignal_AlertEvent(ObjectAEvents::Event_beforeExecute, (std::function<void()>)[]() {std::cout << "SLOT: Before execution!\n"; });
    eventEmitter.OnSignal_AlertEvent(ObjectAEvents::Event_afterExecute, (std::function<void()>)[]() {std::cout << "SLOT: After execution!\n"; });
    eventEmitter.Execute();
    std::cout << "------- END event emission test --------\n\n";

    std::cout << "------- BEGIN value changed test --------\n";
    auto obj1 = std::make_shared<ObjectB>();
    auto obj2 = std::make_shared<ObjectB>();
    SomeProcess process{ obj1, obj2 };
    obj1->OnSignal_ValueChanged([](const int& i) {std::cout << "Object 1 data: " << i << '\n'; });
    obj2->OnSignal_ValueChanged([](const int& i) {std::cout << "Object 2 data: " << i << '\n'; });
    process.Run();
    std::cout << "------- END value changed test --------\n\n";

    std::cout << "------- BEGIN value filtered test --------\n";
    obj1->OnSignal_FilterValue([](int& i) { i *= 2; }); //  0 -> 20
    obj1->OnSignal_FilterValue([](int& i) { i -= 1; }); // -1 -> 19
    obj2->OnSignal_FilterValue([](int& i) { i = 0; }); //  Always 0
    process.Run();
    std::cout << "------- END value filtered test --------\n";

    return 0;
}
