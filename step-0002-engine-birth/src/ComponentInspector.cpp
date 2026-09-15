#include "ComponentInspector.hpp"

#include "ComponentArray.hpp"
#include "ComponentManager.hpp"

#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"

#include <algorithm>
#include <array>
#include <cctype>
#include <cstdio>
#include <cstring>
#include <string>
#include <string_view>

namespace
{
    template <typename T>
    T ReadValue(const std::byte *bytes)
    {
        T value{};
        std::memcpy(&value, bytes, sizeof(T));
        return value;
    }

    template <typename T>
    void WriteValue(std::byte *bytes, T value)
    {
        std::memcpy(bytes, &value, sizeof(T));
    }

    bool ParseHex(std::string_view input, std::byte *bytes, std::size_t size)
    {
        std::string digits;
        for (const char character : input)
        {
            if (std::isxdigit(static_cast<unsigned char>(character)))
                digits += character;
            else if (!std::isspace(static_cast<unsigned char>(character)) && character != '_' && character != '-')
                return false;
        }

        if (digits.size() != size * 2)
            return false;

        for (std::size_t index = 0; index < size; ++index)
        {
            const auto nibble = [](char character) -> unsigned char
            {
                if (character >= '0' && character <= '9') return static_cast<unsigned char>(character - '0');
                if (character >= 'a' && character <= 'f') return static_cast<unsigned char>(character - 'a' + 10);
                return static_cast<unsigned char>(character - 'A' + 10);
            };
            bytes[index] = std::byte{static_cast<unsigned char>(
                (nibble(digits[index * 2]) << 4) | nibble(digits[index * 2 + 1]))};
        }
        return true;
    }

    bool ParseBinary(std::string_view input, std::byte *bytes, std::size_t size)
    {
        std::uint64_t value = 0;
        std::size_t bitCount = 0;
        for (const char character : input)
        {
            if (character == '0' || character == '1')
            {
                if (bitCount == size * 8)
                    return false;
                value = (value << 1) | static_cast<std::uint64_t>(character - '0');
                ++bitCount;
            }
            else if (!std::isspace(static_cast<unsigned char>(character)) && character != '_' && character != '-')
            {
                return false;
            }
        }

        if (bitCount == 0)
            return false;
        std::memcpy(bytes, &value, size);
        return true;
    }

    bool DrawNumericEditor(const char *label, std::byte *bytes, std::size_t size,
        step_0002::ComponentValueFormat format)
    {
        if (format == step_0002::ComponentValueFormat::Float)
        {
            if (size == 4)
            {
                float value = ReadValue<float>(bytes);
                if (ImGui::InputFloat(label, &value)) { WriteValue(bytes, value); return true; }
            }
            else if (size == 8)
            {
                double value = ReadValue<double>(bytes);
                if (ImGui::InputDouble(label, &value)) { WriteValue(bytes, value); return true; }
            }
            return false;
        }

        const bool isSigned = format == step_0002::ComponentValueFormat::SignedInteger;
        if (isSigned)
        {
            if (size == 1) { auto v = ReadValue<std::int8_t>(bytes); if (ImGui::InputScalar(label, ImGuiDataType_S8, &v)) { WriteValue(bytes, v); return true; } }
            if (size == 2) { auto v = ReadValue<std::int16_t>(bytes); if (ImGui::InputScalar(label, ImGuiDataType_S16, &v)) { WriteValue(bytes, v); return true; } }
            if (size == 4) { auto v = ReadValue<std::int32_t>(bytes); if (ImGui::InputScalar(label, ImGuiDataType_S32, &v)) { WriteValue(bytes, v); return true; } }
            if (size == 8) { auto v = ReadValue<std::int64_t>(bytes); if (ImGui::InputScalar(label, ImGuiDataType_S64, &v)) { WriteValue(bytes, v); return true; } }
        }
        else
        {
            if (size == 1) { auto v = ReadValue<std::uint8_t>(bytes); if (ImGui::InputScalar(label, ImGuiDataType_U8, &v)) { WriteValue(bytes, v); return true; } }
            if (size == 2) { auto v = ReadValue<std::uint16_t>(bytes); if (ImGui::InputScalar(label, ImGuiDataType_U16, &v)) { WriteValue(bytes, v); return true; } }
            if (size == 4) { auto v = ReadValue<std::uint32_t>(bytes); if (ImGui::InputScalar(label, ImGuiDataType_U32, &v)) { WriteValue(bytes, v); return true; } }
            if (size == 8) { auto v = ReadValue<std::uint64_t>(bytes); if (ImGui::InputScalar(label, ImGuiDataType_U64, &v)) { WriteValue(bytes, v); return true; } }
        }
        return false;
    }

    std::string FormatValue(const std::byte *bytes, std::size_t size,
        step_0002::ComponentValueFormat format)
    {
        char buffer[96]{};
        if (format == step_0002::ComponentValueFormat::Hex)
        {
            std::string result;
            for (std::size_t i = 0; i < size; ++i)
            {
                std::snprintf(buffer, sizeof(buffer), "%02X%s",
                    std::to_integer<unsigned int>(bytes[i]), i + 1 == size ? "" : " ");
                result += buffer;
            }
            return result;
        }

        if (format == step_0002::ComponentValueFormat::Binary)
        {
            std::string result;
            for (std::size_t i = 0; i < size; ++i)
            {
                const unsigned int value = std::to_integer<unsigned int>(bytes[i]);
                for (int bit = 7; bit >= 0; --bit)
                    result += (value & (1u << bit)) ? '1' : '0';
                if (i + 1 != size) result += ' ';
            }
            return result;
        }

        if (format == step_0002::ComponentValueFormat::Ascii)
        {
            std::string result;
            for (std::size_t i = 0; i < size; ++i)
            {
                const char character = static_cast<char>(std::to_integer<unsigned char>(bytes[i]));
                result += std::isprint(static_cast<unsigned char>(character)) ? character : '.';
            }
            return result;
        }

        if (format == step_0002::ComponentValueFormat::Float && (size == 4 || size == 8))
        {
            if (size == 4) std::snprintf(buffer, sizeof(buffer), "%g", ReadValue<float>(bytes));
            else std::snprintf(buffer, sizeof(buffer), "%g", ReadValue<double>(bytes));
            return buffer;
        }

        if (size != 1 && size != 2 && size != 4 && size != 8)
            return "<choose 1, 2, 4, or 8 bytes>";

        const bool signedFormat = format == step_0002::ComponentValueFormat::SignedInteger;
        if (signedFormat)
        {
            if (size == 1) std::snprintf(buffer, sizeof(buffer), "%d", ReadValue<std::int8_t>(bytes));
            if (size == 2) std::snprintf(buffer, sizeof(buffer), "%d", ReadValue<std::int16_t>(bytes));
            if (size == 4) std::snprintf(buffer, sizeof(buffer), "%d", ReadValue<std::int32_t>(bytes));
            if (size == 8) std::snprintf(buffer, sizeof(buffer), "%lld", static_cast<long long>(ReadValue<std::int64_t>(bytes)));
        }
        else
        {
            if (size == 1) std::snprintf(buffer, sizeof(buffer), "%u", ReadValue<std::uint8_t>(bytes));
            if (size == 2) std::snprintf(buffer, sizeof(buffer), "%u", ReadValue<std::uint16_t>(bytes));
            if (size == 4) std::snprintf(buffer, sizeof(buffer), "%u", ReadValue<std::uint32_t>(bytes));
            if (size == 8) std::snprintf(buffer, sizeof(buffer), "%llu", static_cast<unsigned long long>(ReadValue<std::uint64_t>(bytes)));
        }
        return buffer;
    }
}

namespace step_0002
{
    void ComponentInspector::LoadSelectedSlot(const ComponentArray &componentArray)
    {
        if (selectedSlot >= componentArray.SlotCount())
        {
            editBuffer.clear();
            return;
        }

        const auto &data = componentArray.GetArray();
        const std::size_t offset = selectedSlot * componentArray.GetItemSize();
        editBuffer.assign(data.begin() + offset, data.begin() + offset + componentArray.GetItemSize());
    }

    void ComponentInspector::Draw(ComponentManager &componentManager)
    {
        const auto entries = componentManager.GetComponentArrays();
        ImGui::Begin("Component Inspector");

        if (ImGui::Button("Create entity slot"))
        {
            componentManager.EntityCreated();
        }
        ImGui::SameLine();
        ImGui::TextDisabled("Reserves one slot in every registered component array.");

        if (entries.empty())
        {
            ImGui::TextUnformatted("No component arrays are registered.");
            ImGui::End();
            return;
        }

        const ComponentArrayDebugEntry *selectedEntry = nullptr;
        if (selectedComponentId == 0)
            selectedComponentId = entries.front().id;

        for (const auto &entry : entries)
        {
            const bool selected = entry.id == selectedComponentId;
            if (ImGui::Selectable((entry.name + "##" + std::to_string(entry.id)).c_str(), selected))
            {
                selectedComponentId = entry.id;
                selectedSlot = 0;
                LoadSelectedSlot(*entry.array);
            }
            if (selected) selectedEntry = &entry;
        }

        if (selectedEntry == nullptr)
        {
            selectedComponentId = entries.front().id;
            selectedEntry = &entries.front();
            selectedSlot = 0;
            LoadSelectedSlot(*selectedEntry->array);
        }

        ComponentArray &componentArray = *selectedEntry->array;
        ImGui::Separator();
        ImGui::Text("ID: %u | itemSize: %zu bytes | slots: %zu", selectedEntry->id,
            componentArray.GetItemSize(), componentArray.SlotCount());

        const char *formats[] = { "Hex", "Binary", "Signed integer", "Unsigned integer", "Float", "ASCII / string" };
        int formatIndex = static_cast<int>(format);
        if (ImGui::Combo("Display", &formatIndex, formats, IM_ARRAYSIZE(formats)))
        {
            format = static_cast<ComponentValueFormat>(formatIndex);
            if (format == ComponentValueFormat::Float && groupSize != 4 && groupSize != 8)
                groupSize = 4;
        }

        const std::array<std::size_t, 4> groupSizes{1, 2, 4, 8};
        int groupIndex = 0;
        for (int i = 0; i < static_cast<int>(groupSizes.size()); ++i)
            if (groupSizes[i] == groupSize) groupIndex = i;
        const char *groups[] = { "1 byte", "2 bytes", "4 bytes", "8 bytes" };
        if (ImGui::Combo("Group columns", &groupIndex, groups, IM_ARRAYSIZE(groups)))
            groupSize = groupSizes[groupIndex];

        const std::size_t columnCount = (componentArray.GetItemSize() + groupSize - 1) / groupSize;
        if (ImGui::BeginTable("Component data", static_cast<int>(columnCount + 2),
            ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY,
            ImVec2(0.0f, 320.0f)))
        {
            ImGui::TableSetupColumn("Slot", ImGuiTableColumnFlags_WidthFixed, 56.0f);
            ImGui::TableSetupColumn("Active", ImGuiTableColumnFlags_WidthFixed, 58.0f);
            for (std::size_t column = 0; column < columnCount; ++column)
            {
                const std::string label = "offset +" + std::to_string(column * groupSize);
                ImGui::TableSetupColumn(label.c_str());
            }
            ImGui::TableHeadersRow();

            const auto &data = componentArray.GetArray();
            for (std::size_t slot = 0; slot < componentArray.SlotCount(); ++slot)
            {
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                if (ImGui::Selectable((std::to_string(slot) + "##slot").c_str(), selectedSlot == slot,
                    ImGuiSelectableFlags_SpanAllColumns))
                {
                    selectedSlot = slot;
                    LoadSelectedSlot(componentArray);
                }
                ImGui::TableSetColumnIndex(1);
                bool active = componentArray.IsActive(slot);
                if (ImGui::Checkbox(("##active" + std::to_string(slot)).c_str(), &active))
                    componentArray.SetActive(slot, active);

                const std::size_t baseOffset = slot * componentArray.GetItemSize();
                for (std::size_t column = 0; column < columnCount; ++column)
                {
                    ImGui::TableSetColumnIndex(static_cast<int>(column + 2));
                    const std::size_t offset = column * groupSize;
                    const std::size_t bytes = std::min(groupSize, componentArray.GetItemSize() - offset);
                    ImGui::TextUnformatted(FormatValue(data.data() + baseOffset + offset, bytes, format).c_str());
                }
            }
            ImGui::EndTable();
        }

        if (selectedSlot < componentArray.SlotCount())
        {
            ImGui::Separator();
            ImGui::Text("Edit %s: slot %zu", formats[formatIndex], selectedSlot);
            if (editBuffer.size() != componentArray.GetItemSize())
                LoadSelectedSlot(componentArray);

            const bool usesTextFields = format == ComponentValueFormat::Hex
                || format == ComponentValueFormat::Binary
                || format == ComponentValueFormat::Ascii;
            if (usesTextFields && (textEditorComponentId != selectedEntry->id
                || textEditorSlot != selectedSlot
                || textEditorGroupSize != groupSize
                || textEditorFormat != format))
            {
                textFields.clear();
                if (format == ComponentValueFormat::Ascii)
                {
                    std::string text;
                    for (const std::byte byte : editBuffer)
                    {
                        const char character = static_cast<char>(std::to_integer<unsigned char>(byte));
                        text += std::isprint(static_cast<unsigned char>(character)) ? character : ' ';
                    }
                    textFields.push_back(std::move(text));
                }
                else
                {
                    for (std::size_t offset = 0; offset < editBuffer.size(); offset += groupSize)
                    {
                        const std::size_t bytes = std::min(groupSize, editBuffer.size() - offset);
                        textFields.push_back(FormatValue(editBuffer.data() + offset, bytes, format));
                    }
                }
                textEditorComponentId = selectedEntry->id;
                textEditorSlot = selectedSlot;
                textEditorGroupSize = groupSize;
                textEditorFormat = format;
            }

            if (format == ComponentValueFormat::Ascii)
            {
                ImGui::TextDisabled("Text is stored as ASCII bytes; unused bytes are set to zero.");
                ImGui::SetNextItemWidth(-1.0f);
                if (ImGui::InputText("##ascii", &textFields[0]))
                {
                    std::fill(editBuffer.begin(), editBuffer.end(), std::byte{0});
                    const std::size_t count = std::min(editBuffer.size(), textFields[0].size());
                    std::memcpy(editBuffer.data(), textFields[0].data(), count);
                }
            }
            else
            {
                for (std::size_t offset = 0, field = 0; offset < editBuffer.size(); offset += groupSize, ++field)
                {
                    const std::size_t bytes = std::min(groupSize, editBuffer.size() - offset);
                    const std::string label = "offset +" + std::to_string(offset) + "##editor" + std::to_string(offset);
                    ImGui::SetNextItemWidth(170.0f);

                    if (format == ComponentValueFormat::Hex || format == ComponentValueFormat::Binary)
                    {
                        const ImGuiInputTextFlags flags = format == ComponentValueFormat::Hex
                            ? ImGuiInputTextFlags_CharsHexadecimal : ImGuiInputTextFlags_CharsDecimal;
                        if (ImGui::InputText(label.c_str(), &textFields[field], flags))
                        {
                            const bool valid = format == ComponentValueFormat::Hex
                                ? ParseHex(textFields[field], editBuffer.data() + offset, bytes)
                                : ParseBinary(textFields[field], editBuffer.data() + offset, bytes);
                            if (!valid)
                                ImGui::SetTooltip("Keep %zu %s.", bytes,
                                    format == ComponentValueFormat::Hex ? "complete byte(s), e.g. FF 0A" : "binary byte(s), e.g. 11001010");
                        }
                    }
                    else if (!DrawNumericEditor(label.c_str(), editBuffer.data() + offset, bytes, format))
                    {
                        ImGui::TextDisabled("%s requires a 1, 2, 4, or 8-byte group.", formats[formatIndex]);
                    }
                }
            }

            if (ImGui::Button("Apply edited value"))
                componentManager.UpdateComponent(selectedSlot, selectedEntry->id, editBuffer.data());
            ImGui::SameLine();
            if (ImGui::Button("Add / activate component"))
                componentManager.AddComponent(selectedSlot, selectedEntry->id, editBuffer.data());
            ImGui::SameLine();
            if (ImGui::Button("Remove component"))
                componentManager.RemoveComponent(selectedEntry->id, selectedSlot);
        }

        ImGui::End();
    }
}
