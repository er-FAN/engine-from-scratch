#pragma once
#include <cstddef>
#include <vector>

namespace step_0002
{
    class ComponentArray
    {
    private:
        std::vector<std::byte> data;
        std::vector<std::byte> activeMask;
        std::size_t itemSize = 1;

    public:
        explicit ComponentArray(std::size_t itemSize);
        ~ComponentArray();

        std::size_t SlotCount() const;
        std::size_t LastSlotIndex() const;
        std::size_t GetItemSize() const;

        const std::vector<std::byte>& GetArray() const;
        const std::vector<std::byte>& GetActiveMask() const;

        bool Reserve();
        bool Update(std::size_t index, const void *value);
        bool SetActive(std::size_t index, bool isActive);
        bool IsActive(std::size_t index) const;
    };
}
