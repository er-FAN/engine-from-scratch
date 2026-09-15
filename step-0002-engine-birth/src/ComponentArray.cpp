#include "ComponentArray.hpp"
#include <algorithm>
#include <cstring>
#include <stdexcept>

namespace step_0002
{
    std::size_t ComponentArray::SlotCount() const
    {
        return data.size() / itemSize;
    }

    std::size_t ComponentArray::LastSlotIndex() const
    {
        if (SlotCount() == 0)
        {
            return 0;
        }
        return SlotCount() - 1;
    }

    std::size_t ComponentArray::GetItemSize() const
    {
        return itemSize;
    }

    ComponentArray::ComponentArray(std::size_t itemSize)
        : itemSize(itemSize)
    {
        if (itemSize == 0)
        {
            throw std::invalid_argument("ComponentArray itemSize must be greater than zero");
        }
    }

    ComponentArray::~ComponentArray() = default;

    const std::vector<std::byte> &ComponentArray::GetArray() const
    {
        return ComponentArray::data;
    }

    const std::vector<std::byte> &ComponentArray::GetActiveMask() const
    {
        return ComponentArray::activeMask;
    }

    bool ComponentArray::Reserve()
    {
        // Reserve space for one component
        data.insert(
            data.end(),
            itemSize,
            std::byte{0});

        // Add a new bit every 8 components
        std::size_t componentIndex = LastSlotIndex();

        std::size_t byteIndex = componentIndex / 8;

        if (byteIndex >= activeMask.size())
        {
            activeMask.push_back(std::byte{0});
        }

        return true;
    }

    bool ComponentArray::Update(std::size_t index, const void *value)
    {
        if (index >= SlotCount() || value == nullptr)
            return false;

        const std::size_t byteIndex = index * itemSize;
        std::memcpy(data.data() + byteIndex, value, itemSize);

        return true;
    }

    bool ComponentArray::SetActive(std::size_t index, bool active)
    {
        if (index >= SlotCount())
        {
            return false;
        }

        std::size_t byteIndex = index / 8;
        std::size_t bitIndex = index % 8;

        if (active)
        {
            activeMask[byteIndex] |=
                std::byte{1} << bitIndex;
        }
        else
        {
            activeMask[byteIndex] &=
                ~(std::byte{1} << bitIndex);
        }

        return true;
    }

    bool ComponentArray::IsActive(std::size_t index) const
    {
        if (index >= SlotCount())
        {
            return false;
        }

        std::size_t byteIndex = index / 8;
        std::size_t bitIndex = index % 8;

        bool isActive =
            (activeMask[byteIndex] & (std::byte{1} << bitIndex)) != std::byte{0};

        return isActive;
    }
}
