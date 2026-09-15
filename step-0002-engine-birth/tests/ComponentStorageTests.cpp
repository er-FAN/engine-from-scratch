#include "ComponentArray.hpp"
#include "ComponentManager.hpp"

#include <cassert>
#include <cstdint>
#include <cstring>
#include <stdexcept>

int main()
{
    using namespace step_0002;

    bool rejectedZeroSize = false;
    try
    {
        ComponentArray invalid(0);
    }
    catch (const std::invalid_argument &)
    {
        rejectedZeroSize = true;
    }
    assert(rejectedZeroSize);

    ComponentArray array(sizeof(std::int32_t));
    assert(array.Reserve());
    assert(array.Reserve());
    const std::int32_t value = 0x12345678;
    assert(array.Update(1, &value));
    assert(!array.Update(2, &value));
    assert(!array.Update(1, nullptr));
    std::int32_t stored{};
    std::memcpy(&stored, array.GetArray().data() + sizeof(std::int32_t), sizeof(stored));
    assert(stored == value);
    assert(array.SetActive(1, true));
    assert(array.IsActive(1));
    assert(array.SetActive(1, false));
    assert(!array.IsActive(1));

    ComponentManager manager;
    assert(manager.RegisterComponent(7, sizeof(std::int32_t), "Test value"));
    assert(!manager.RegisterComponent(7, sizeof(std::int32_t)));
    assert(manager.EntityCreated());
    assert(manager.EntityCreated());
    assert(manager.AddComponent(1, 7, &value));
    assert(!manager.AddComponent(1, 99, &value));

    const auto entries = manager.GetComponentArrays();
    assert(entries.size() == 1);
    assert(entries[0].id == 7);
    assert(entries[0].name == "Test value");
    assert(entries[0].array->SlotCount() == 2);
    assert(entries[0].array->IsActive(1));
}
