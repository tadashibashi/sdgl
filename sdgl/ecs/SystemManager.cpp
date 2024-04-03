#include "SystemManager.h"

namespace sdgl {
    SystemManager::SystemManager() : m_systems(
        [](const ISystem *sys) { return sys->initPriority(); },
        [](ISystem * sys) { sys->shutdown(); delete sys; })
    { }

    bool SystemManager::init()
    {
        for (const auto sys : m_systems)
        {
            if (!sys->init())
            {
                // shutdown systems that were initialized for good measure ??
                for (const auto toShutdown : m_systems)
                {
                    if (toShutdown == sys)
                        return false;
                    toShutdown->shutdown();
                }

                return false;
            }
        }

        return true;
    }

    void SystemManager::shutdown()
    {
        for (const auto sys : m_systems)
        {
            sys->shutdown();
        }
    }

}

