#pragma once
#include "Entity.h"

namespace sdgl::ecs {
    /// Thin wrapper around an `entt::registry`
    class World {
    public:
        /// Should not be copiable
        World(const World &) = delete;
        World &operator=(const World &) = delete;

        /// Create an entity (or recycle an old one)
        Entity create();

        /// Destroy an entity
        void destroy(const Entity &entity);

        /// Create a view by which to observe entities with component sets
        template<typename ...T>
        auto view() { return m_registry.view<T...>(); }
    private:
        entt::registry m_registry;
    };
}
