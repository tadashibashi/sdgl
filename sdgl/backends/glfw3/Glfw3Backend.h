#pragma once

#include <sdgl/backends/Backend.h>

namespace sdgl::backends::glfw3 {
    class Glfw3Backend final : public Backend {
    public:
        Glfw3Backend();
        ~Glfw3Backend() override;

        bool init() override;

        [[nodiscard]]
        double getAppTime() const override;

        [[nodiscard]]
        string_view name() const override { return "GLFW3"; }

        Window *createWindow(const string &title, int width, int height, WindowFlags::Enum flags, const PluginConfig &plugins) override;
        void destroyWindow(Window *window) override;

        void processInput() override;
        void shutdown() override;
    private:
        struct Impl;
        Impl *m;
    };
}
