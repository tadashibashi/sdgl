#pragma once
#include "World.h"

namespace sdgl {
    /**
     * System for a component
     */
    class ISystem {
    public:
        explicit ISystem(ecs::World &world) : m_world(world) { }
        virtual ~ISystem() = default;
        virtual bool init() = 0;
        virtual void shutdown() = 0;
        virtual void update(double deltaTime) = 0;

        /** Higher occurs earlier, lower occurs later */
        [[nodiscard]]
        virtual int initPriority() const { return 0; }
        [[nodiscard]]
        virtual int updatePriority() const { return 0; }

    protected:
        [[nodiscard]]
        const ecs::World &getWorld() const { return m_world; }
        [[nodiscard]]
        ecs::World &getworld() { return m_world; }

    private:
        ecs::World &m_world;
    };
}
