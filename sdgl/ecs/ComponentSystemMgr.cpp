#include "ComponentSystemMgr.h"

#include "sdgl/logging.h"

namespace sdgl {
    bool ComponentSystemMgr::init()
    {
        if (m_wasInit)
        {
            SDGL_WARN("Called init, but ComponentSystemManager was already init");
        }

        for (const auto sys : m_systems)
        {
            if (!sys->init())
            {
                return false;
            }
        }

        m_wasInit = true;
        return true;
    }

    void ComponentSystemMgr::startFrame()
    {
        for (const auto frameSys : m_frameSystems)
            frameSys->updateStart();
    }

    void ComponentSystemMgr::endFrame()
    {
        for (const auto frameSys : std::ranges::reverse_view(m_frameSystems))
            frameSys->updateEnd();
    }

    void ComponentSystemMgr::render()
    {
        for (const auto renderSys : m_renderSystems)
            renderSys->startRender();
    }

    void ComponentSystemMgr::shutdown()
    {
        for (const auto sys : std::ranges::reverse_view(m_systems))
        {
            sys->shutdown();
            delete sys;
        }

        m_frameSystems.clear();
        m_renderSystems.clear();
        m_systems.clear();
        m_wasInit = false;
    }



    bool ComponentSystemMgr::addSystem(ISystem *system)
    {
        if (!system)
        {
            return false;
        }

        try
        {
            if (auto frameSystem = dynamic_cast<IUpdateSystem *>(system))
            {
                if (m_frameSystems.empty())
                {
                    m_frameSystems.emplace_back(frameSystem);
                }
                else
                {
                    bool wasAdded = false;
                    for (auto it = m_frameSystems.begin(); it != m_frameSystems.end(); ++it)
                    {
                        if (frameSystem->updatePriority() > (*it)->updatePriority())
                        {
                            m_frameSystems.insert(it, frameSystem);
                            wasAdded = true;
                            break;
                        }
                    }
                    if (!wasAdded)
                        m_frameSystems.emplace_back(frameSystem);
                }
            }

            if (auto renderSystem = dynamic_cast<IRenderSystem *>(system))
            {
                if (m_renderSystems.empty())
                {
                    m_renderSystems.emplace_back(renderSystem);
                }
                else
                {
                    bool wasAdded = false;
                    for (auto it = m_renderSystems.begin(); it != m_renderSystems.end(); ++it)
                    {
                        if (renderSystem->renderPriority() > (*it)->renderPriority())
                        {
                            m_renderSystems.insert(it, renderSystem);
                            wasAdded = true;
                            break;
                        }
                    }
                    if (!wasAdded)
                        m_renderSystems.emplace_back(renderSystem);
                }
            }

            // add to plugin list
            {
                bool wasAdded = false;
                for (auto it = m_systems.begin(); it != m_systems.end(); ++it)
                {
                    if (system->initPriority() > (*it)->initPriority())
                    {
                        m_systems.insert(it, system);
                        wasAdded = true;
                        break;
                    }
                }
                if (!wasAdded)
                    m_systems.emplace_back(system);

                if (m_wasInit)
                {
                    system->init();
                }
            }

            return true;
        }
        catch(...)
        {
            removeSystem(system);
            return false;
        }
    }

    bool ComponentSystemMgr::addSystems(const vector<ISystem *> &systems)
    {

        for (const auto sys : systems)
        {
            if (!addSystem(sys))
                return false;
        }

        return true;
    }

    bool ComponentSystemMgr::eraseSystems(const vector<ISystem *>::iterator begin)
    {
        if (begin == m_systems.end()) return false;

        for (auto toRemoveIt = begin; toRemoveIt != m_systems.end(); ++toRemoveIt)
        {
            // erase if frame system
            if (auto frameSys = dynamic_cast<IUpdateSystem *>(*toRemoveIt))
            {
                if (auto frameSysIt = std::ranges::find(m_frameSystems, frameSys);
                    frameSysIt != m_frameSystems.end())
                {
                    m_frameSystems.erase(frameSysIt);
                }
            }

            // erase if render system
            if (auto renderSys = dynamic_cast<IRenderSystem *>(*toRemoveIt))
            {
                if (auto renderSysIt = std::ranges::find(m_renderSystems, renderSys);
                    renderSysIt != m_renderSystems.end())
                {
                    m_renderSystems.erase(renderSysIt);
                }
            }

            (*toRemoveIt)->shutdown();
            delete *toRemoveIt;
        }

        return true;
    }
}
