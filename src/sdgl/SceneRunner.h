#pragma once
#include <stack>
#include <variant>
#include "ServiceContainer.h"

namespace sdgl {
    class Scene;

    class SceneRunner {
        struct StartCommand
        {
            Scene *scene;        ///< scene to start
            bool replaceCurrent; ///< whether to end current scene before starting next one
        };

        struct StopCommand
        {
            Scene *scene; ///< scene to end
        };

        using SceneCommand = std::variant<StartCommand, StopCommand>;

    public:
        SceneRunner() = default;

        [[nodiscard]]
        const auto &services() const { return m_services; }
        auto &services() { return m_services; }

        template <typename T> requires
            std::is_base_of_v<Scene, T> && std::is_default_constructible_v<T>
        void startScene(bool stopCurrent = true)
        {
            m_commands.emplace_back(StartCommand {
                .scene = new T(),
                .replaceCurrent = stopCurrent
            });
        }

        /// Stop the current scene
        /// @note this command will succeed if the current scene is the same
        /// when the command runs. If another command ends the scene, or begins a
        /// new one before this command in the scene command queue this frame,
        /// then it will do nothing.
        ///
        void stopScene()
        {
            if (m_current.empty())
                return;

            m_commands.emplace_back(StopCommand {
                .scene = m_current.top()
            });
        }

        void update(float deltaTime);
        void render(float deltaTime);

    private:
        void applyChanges();

        std::stack<Scene *> m_current;

        ServiceContainer m_services;
        vector<SceneCommand> m_commands;
    };

} // sdgl
