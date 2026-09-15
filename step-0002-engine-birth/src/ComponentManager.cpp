#include "ComponentManager.hpp"

#include <utility>

step_0002::ComponentManager::ComponentManager() = default;

step_0002::ComponentManager::~ComponentManager() = default;

bool step_0002::ComponentManager::EntityCreated()
{
    for (auto &componentArray : ComponentManager::componentArrays)
    {
        componentArray.second.Reserve();
    }

    return true;
}

bool step_0002::ComponentManager::EntityDestroyed(std::size_t index)
{
    for (auto &componentArray : ComponentManager::componentArrays)
    {
        componentArray.second.SetActive(index, false);
    }

    return true;
}

bool step_0002::ComponentManager::RegisterComponent(
    uint32_t id,
    std::size_t itemSize,
    std::string debugName)
{
    if (componentArrays.find(id) != componentArrays.end())
    {
        return false;
    }

    componentArrays.emplace(id, ComponentArray(itemSize));
    componentNames.emplace(
        id,
        debugName.empty() ? "Component " + std::to_string(id) : std::move(debugName));

    return true;
}

bool step_0002::ComponentManager::AddComponent(std::size_t index, uint32_t componentArrayId, const void *value)
{
    auto componentArray = componentArrays.find(componentArrayId);
    if (componentArray == componentArrays.end())
    {
        return false;
    }

    if (!componentArray->second.Update(index, value))
    {
        return false;
    }

    return componentArray->second.SetActive(index, true);
}

bool step_0002::ComponentManager::RemoveComponent(uint32_t componentArrayId, std::size_t index)
{
    auto componentArray = componentArrays.find(componentArrayId);
    return componentArray != componentArrays.end()
        && componentArray->second.SetActive(index, false);
}

bool step_0002::ComponentManager::UpdateComponent(std::size_t index, uint32_t componentArrayId, const void *value)
{
    auto componentArray = componentArrays.find(componentArrayId);
    if (componentArray == componentArrays.end())
    {
        return false;
    }

    if (!componentArray->second.Update(index, value))
    {
        return false;
    }

    return componentArray->second.SetActive(index, true);
}

std::vector<step_0002::ComponentArrayDebugEntry>
step_0002::ComponentManager::GetComponentArrays()
{
    std::vector<ComponentArrayDebugEntry> entries;
    entries.reserve(componentArrays.size());

    for (auto &componentArray : componentArrays)
    {
        entries.push_back({
            componentArray.first,
            componentNames.at(componentArray.first),
            &componentArray.second});
    }

    return entries;
}
