#pragma once
#include <cstdint>
#include <vector>

namespace step_0002
{
    class ComponentArray
    {
    private:
        std::vector<std::byte> data;
    public:
        ComponentArray();
        ~ComponentArray();

        ComponentArray GetComponentArray();

        bool Add(std::vector<std::byte> data);
        bool Update(uint64_t fromIndex, std::vector<std::byte> data);
        bool Remove(uint64_t fromIndex, uint64_t lenght);
    };
}