#include "World.h"

namespace sdgl::ecs {
    Entity World::create()
    {
        return {m_registry.create(), m_registry};
    }

    void World::destroy(const Entity &entity)
    {
        m_registry.destroy(entity.id());
    }
}
