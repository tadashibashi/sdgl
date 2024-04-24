#include "SceneRunner.h"
#include "Scene.h"

namespace sdgl {
    void SceneRunner::update(float deltaTime)
    {
        if (!m_current.empty())
            m_current.top()->update(deltaTime);
        applyChanges();
    }

    void SceneRunner::render(float deltaTime)
    {
        if (!m_current.empty())
            m_current.top()->render(deltaTime);
    }

    void SceneRunner::applyChanges()
    {
        if (m_commands.empty())
            return;

        for (auto &command : m_commands)
        {
            switch(command.index())
            {
                case 0: // Start command
                {
                    auto &[scene, replaceCurrent] = std::get<StartCommand>(command);
                    if (!scene)
                        break;

                    // deal with current scene
                    if (!m_current.empty())
                    {
                        if (replaceCurrent)
                        {
                            m_current.top()->shutdown();
                            m_current.pop();
                        }
                        else
                        {
                            m_current.top()->paused();
                        }
                    }

                    // apply new scene
                    scene->m_manager = this;
                    scene->init();
                    m_current.emplace(scene);
                }
                    break;

                case 1: // Stop command
                {
                    auto &[scene] = std::get<StopCommand>(command);

                    if (m_current.empty() || m_current.top() != scene)
                        break;

                    // clean up and pop current scene
                    m_current.top()->shutdown();
                    m_current.pop();

                    // check for any underlying scenes to notify it got resumed
                    if (!m_current.empty())
                    {
                        m_current.top()->resumed();
                    }
                }
                    break;

                default:
                    throw std::runtime_error("Invalid scene command type");
            }
        }

        m_commands.clear();
    }
} // sdgl