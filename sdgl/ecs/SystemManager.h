#pragma once
#include "ISystem.h"
#include "PriorityList.h"

namespace sdgl {
    class SystemManager final : public ISystem
    {
    public:
        SystemManager();
        ~SystemManager() override = default;

        bool init() override;
        void shutdown() override;

        void add(ISystem *system);
        bool erase(ISystem *system);
    private:
        PriorityList<ISystem *> m_systems;
    };
}
