#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace step_0002
{
    class ComponentManager;

    enum class ComponentValueFormat
    {
        Hex,
        Binary,
        SignedInteger,
        UnsignedInteger,
        Float,
        Ascii
    };

    // A Dear ImGui panel. Call Draw() once per ImGui frame.
    class ComponentInspector
    {
    public:
        void Draw(ComponentManager &componentManager);

    private:
        uint32_t selectedComponentId = 0;
        std::size_t selectedSlot = 0;
        std::size_t groupSize = 1;
        ComponentValueFormat format = ComponentValueFormat::Hex;
        std::vector<std::byte> editBuffer;
        uint32_t textEditorComponentId = 0;
        std::size_t textEditorSlot = 0;
        std::size_t textEditorGroupSize = 0;
        ComponentValueFormat textEditorFormat = ComponentValueFormat::Hex;
        std::vector<std::string> textFields;

        void LoadSelectedSlot(const class ComponentArray &componentArray);
    };
}
