#include "SystemManager.h"

namespace sdgl::ecs {
    SystemManager::SystemManager() :
        m_systems(
            [](const ISystem *sys) { return sys->initPriority(); },
            [](ISystem * sys) { sys->shutdown(); delete sys; }), // erase callback deletes from main systems list only
        m_updateList(
            [](const ISystem *sys) { return sys->updatePriority(); }),
        m_wasInit(false)
    { }

    SystemManager::~SystemManager()
    {
        // Clean up any remaining systems
        shutdown();
    }

    bool SystemManager::init()
    {
        // Only call this once, until shutdown is called
        if (m_wasInit)
        {
            SDGL_WARN("Calling `init` on a SystemManager after it was already initialized has no effect");
            return true;
        }

        // Initialize each system
        for (auto it = m_systems.begin(); it != m_systems.end(); ++it)
        {
            if (!(*it)->init())
            {
                // A system failed to init, shutdown all that were initialized for good measure
                for (auto shutdownIt = it - 1; shutdownIt >= m_systems.begin(); --shutdownIt)
                {
                    (*shutdownIt)->shutdown();
                }

                return false;
            }
        }

        m_wasInit = true;
        return true;
    }

    void SystemManager::shutdown()
    {
        m_systems.clear();
        m_wasInit = false;
    }

    void SystemManager::update(const double deltaTime)
    {
        for (auto &sys : m_updateList)
        {
            sys->update(deltaTime);
        }
    }
}

