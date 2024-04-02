#include "PluginManager.h"

#include <iostream>

#include "logging.h"

namespace sdgl {
    PluginManager::PluginManager(const vector<IPlugin *> &plugins)
    {
        for (const auto plug : plugins)
        {
            addPlugin(plug);
        }
    }

    bool PluginManager::init()
    {
        if (m_wasInit)
        {
            SDGL_WARN("Called init, but PluginManager was already init");
            return true;
        }

        for (auto plug : m_plugs)
        {
            if (!plug->init())
            {
                return false;
            }
        }

        m_wasInit = true;
        return true;
    }

    void PluginManager::addPlugins(const vector<IPlugin *> &plugs)
    {
        for (auto &plug : plugs)
        {
            addPlugin(plug);
        }
    }

    void PluginManager::addPlugin(IPlugin *plug)
    {
        if (!plug)
        {
            return; // don't add a null plugin
        }

        try
        {
            // add if render plugin
            if (auto renderPlug = dynamic_cast<IRenderPlugin *>(plug))
            {
                if (m_renderPlugs.empty())
                {
                    m_renderPlugs.emplace_back(renderPlug);
                }
                else
                {
                    bool wasAdded = false;
                    for (auto it = m_renderPlugs.begin(); it != m_renderPlugs.end(); ++it)
                    {
                        if (renderPlug->renderPriority() > (*it)->renderPriority())
                        {
                            m_renderPlugs.insert(it, renderPlug);
                            wasAdded = true;
                            break;
                        }
                    }
                    if (!wasAdded)
                        m_renderPlugs.emplace_back(renderPlug);
                }

            }

            // add if frame plugin
            if (auto framePlug = dynamic_cast<IFramePlugin *>(plug))
            {
                if (m_framePlugs.empty())
                {
                    m_framePlugs.emplace_back(framePlug);
                }
                else
                {
                    bool wasAdded = false;
                    for (auto it = m_framePlugs.begin(); it != m_framePlugs.end(); ++it)
                    {
                        if (framePlug->framePriority() > (*it)->framePriority())
                        {
                            m_framePlugs.insert(it, framePlug);
                            wasAdded = true;
                            break;
                        }
                    }
                    if (!wasAdded)
                        m_framePlugs.emplace_back(framePlug);
                }
            }

            // add if event plugin
            if (auto eventPlug = dynamic_cast<IEventPlugin *>(plug))
            {
                if (m_eventPlugs.empty())
                {
                    m_eventPlugs.emplace_back(eventPlug);
                }
                else
                {
                    bool wasAdded = false;
                    for (auto it = m_eventPlugs.begin(); it != m_eventPlugs.end(); ++it)
                    {
                        if (eventPlug->eventPriority() > (*it)->eventPriority())
                        {
                            m_eventPlugs.insert(it, eventPlug);
                            wasAdded = true;
                            break;
                        }
                    }
                    if (!wasAdded)
                        m_eventPlugs.emplace_back(eventPlug);
                }
            }

            // add to plugin list
            {
                bool wasAdded = false;
                for (auto it = m_plugs.begin(); it != m_plugs.end(); ++it)
                {
                    if (plug->initPriority() > (*it)->initPriority())
                    {
                        m_plugs.insert(it, plug);
                        wasAdded = true;
                        break;
                    }
                }
                if (!wasAdded)
                    m_plugs.emplace_back(plug);

                if (m_wasInit)
                {
                    plug->init();
                }
            }
        }
        catch(...)
        {
            removePlugin(plug);
        }

    }

    void PluginManager::removePlugin(IPlugin *plugin)
    {
        const auto it = std::remove(m_plugs.begin(), m_plugs.end(), plugin);
        erasePlugins(it);
    }

    void PluginManager::startFrame()
    {
        for (auto framePlug : m_framePlugs)
            framePlug->startFrame();
    }

    void PluginManager::endFrame()
    {
        // fires in reverse order
        for (auto &framePlug : std::ranges::reverse_view(m_framePlugs))
            framePlug->endFrame();
    }

    void PluginManager::shutdown()
    {
        // fires in reverse order from init
        for (auto plug : std::ranges::reverse_view(m_plugs))
        {
            plug->shutdown();
            delete plug;
        }

        m_framePlugs.clear();
        m_renderPlugs.clear();
        m_plugs.clear();
        m_wasInit = false;
    }


    void PluginManager::erasePlugins(const vector<IPlugin *>::iterator begin)
    {
        if (begin != m_plugs.end())
        {
            for (auto toRemoveIt = begin; toRemoveIt != m_plugs.end(); ++toRemoveIt)
            {
                // erase if frame plugin
                if (auto framePlug = dynamic_cast<IFramePlugin *>(*toRemoveIt))
                {
                    if (auto framePlugIt = std::ranges::find(m_framePlugs, framePlug);
                        framePlugIt != m_framePlugs.end())
                    {
                        m_framePlugs.erase(framePlugIt);
                    }
                }

                // erase if render plugin
                if (auto renderPlug = dynamic_cast<IRenderPlugin *>(*toRemoveIt))
                {
                    if (auto renderPlugIt = std::ranges::find(m_renderPlugs, renderPlug);
                        renderPlugIt != m_renderPlugs.end())
                    {
                        m_renderPlugs.erase(renderPlugIt);
                    }
                }

                // erase if event plugin
                if (auto eventPlug = dynamic_cast<IEventPlugin *>(*toRemoveIt))
                {
                    if (auto eventPlugIt = std::ranges::find(m_eventPlugs, eventPlug);
                        eventPlugIt != m_eventPlugs.end())
                    {
                        m_eventPlugs.erase(eventPlugIt);
                    }
                }

                (*toRemoveIt)->shutdown();
                delete *toRemoveIt;
            }

            // erase all
            m_plugs.erase(begin, m_plugs.end());
        }
    }

    void PluginManager::render()
    {
        for (const auto plug : m_renderPlugs)
            plug->render();
    }

    void PluginManager::processEvent(const void *event)
    {
        for (const auto plug : m_eventPlugs)
            plug->processEvent(event);
    }
}
