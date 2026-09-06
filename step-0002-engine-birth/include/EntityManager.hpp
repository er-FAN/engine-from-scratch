#pragma once
#include <vector>

namespace step_0002
{
    class Entity;

    class EntityManager
    {
    private:
        std::vector<Entity> allEntities;
        std::vector<Entity> inactiveEntities;
    public:
        EntityManager();
        ~EntityManager();

        std::vector<Entity> GetAll();
        std::vector<Entity> GetActiveEntities();
        std::vector<Entity> GetInactiveEntities();

        Entity CreateEntity();
        Entity DeactivateEntity(Entity entity);

        bool IsActive(Entity entity);
    };
}
