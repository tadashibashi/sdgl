#pragma once
#include "ISystem.h"
#include <sdgl/sdglib.h>
#include <ranges>

namespace sdgl {
    class ComponentSystemMgr {
    public:
        explicit ComponentSystemMgr() :
            m_systems(), m_frameSystems(), m_renderSystems(), m_wasInit(false)
        { }

        bool init();
        void startFrame();
        void endFrame();
        void render();
        void shutdown();

        bool addSystem(ISystem *system);
        bool addSystems(const vector<ISystem *> &systems);

        template <typename U> requires std::is_base_of_v<ISystem, U>
        bool removeSystem();
        bool removeSystem(ISystem *system) { return eraseSystems(std::ranges::find(m_systems, system)); }

        [[nodiscard]]
        auto size() const { return m_systems.size(); }
        [[nodiscard]]
        auto empty() const { return m_systems.empty(); }
    private:
        bool eraseSystems(vector<ISystem *>::iterator begin);
        vector<ISystem *> m_systems;
        vector<IUpdateSystem *> m_frameSystems;
        vector<IRenderSystem *> m_renderSystems;

        int m_initPriority, m_framePriority, m_renderPriority;
        bool m_wasInit;
    };

    template<typename U> requires std::is_base_of_v<ISystem, U>
    bool ComponentSystemMgr::removeSystem()
    {
        return eraseSystems(std::ranges::remove_if(m_systems, [](ISystem *system) {
            return dynamic_cast<U *>(system);
        }));
    }
}
