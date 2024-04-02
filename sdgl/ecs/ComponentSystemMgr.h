#pragma once
#include "IComponentSystem.h"
#include <sdgl/sdglib.h>
#include <ranges>

namespace sdgl::ecs {
    class ComponentSystemMgr : public IComponentSystem, public IFrameSystem, public IRenderSystem  {
    public:
        explicit ComponentSystemMgr(const int initPriority = 0, const int framePriority = 0, const int renderPriority = 0) :
            m_systems(), m_frameSystems(), m_renderSystems(),
            m_initPriority(initPriority), m_framePriority(framePriority), m_renderPriority(renderPriority),
            m_wasInit(false)
        { }

        bool init() override;
        void startFrame() override;
        void endFrame() override;
        void render() override;
        void shutdown() override;

        bool addSystem(IComponentSystem *system);
        bool addSystems(const vector<IComponentSystem *> &systems);

        template <typename U> requires std::is_base_of_v<IComponentSystem, U>
        bool removeSystem();
        bool removeSystem(IComponentSystem *system) { return eraseSystems(std::ranges::find(m_systems, system)); }

        [[nodiscard]]
        auto size() const { return m_systems.size(); }
        [[nodiscard]]
        auto empty() const { return m_systems.empty(); }

        [[nodiscard]]
        int renderPriority() const override { return m_renderPriority; }
        [[nodiscard]]
        int initPriority() const override { return m_initPriority; }
    private:
        bool eraseSystems(vector<IComponentSystem *>::iterator begin);
        vector<IComponentSystem *> m_systems;
        vector<IFrameSystem *> m_frameSystems;
        vector<IRenderSystem *> m_renderSystems;

        int m_initPriority, m_framePriority, m_renderPriority;
        bool m_wasInit;
    };

    template<typename U> requires std::is_base_of_v<IComponentSystem, U>
    bool ComponentSystemMgr::removeSystem()
    {
        return eraseSystems(std::ranges::remove_if(m_systems, [](IComponentSystem *system) {
            return dynamic_cast<U *>(system);
        }));
    }
}
