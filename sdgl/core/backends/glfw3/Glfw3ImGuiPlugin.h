#pragma once
#include "../../IPlugin.h"

struct ImGuiContext;
class GLFWwindow;

namespace sdgl::backends::glfw3 {

    class Glfw3ImGuiPlugin final : public IPlugin, public IFramePlugin, public IRenderPlugin {
    public:
        explicit Glfw3ImGuiPlugin(GLFWwindow *window) : window(window), m_context{} { }

        bool init() override;

        void startFrame() override;

        void endFrame() override;

        void render() override;

        void shutdown() override;

        ~Glfw3ImGuiPlugin() override;

    private:
        ImGuiContext *m_context;
        GLFWwindow *window;
    };
}
