#include "ComponentArray.hpp"

namespace step_0002
{
    ComponentArray::ComponentArray() = default;

    ComponentArray::~ComponentArray() = default;

    ComponentArray ComponentArray::GetComponentArray()
    {
        return ComponentArray();
    }

    bool ComponentArray::Add(std::vector<std::byte> data)
    {
        return false;
    }

    bool ComponentArray::Update(uint64_t fromIndex, std::vector<std::byte> data)
    {
        return false;
    }

    bool ComponentArray::Remove(uint64_t fromIndex, uint64_t lenght)
    {
        return false;
    }
}
