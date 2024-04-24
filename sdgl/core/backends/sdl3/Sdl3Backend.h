#pragma once
#include <sdgl/core/backends/Backend.h>

struct SDL_Gamepad;

namespace sdgl::backends::sdl3 {
    class Sdl3Window;

    class Sdl3Backend final : public Backend {
    public:
        Sdl3Backend();
        ~Sdl3Backend() override;

        bool init() override;

        [[nodiscard]]
        float getAppTime() const override;

        [[nodiscard]]
        string_view name() const override { return "SDL3"; };

        Window *createWindow(const string &title, int width, int height, WindowFlags::Enum flags, const PluginConfig &plugins) override;

        void destroyWindow(Window *window) override;

        void processInput() override;

        void shutdown() override;
    private:
        vector<Sdl3Window *> m_windows;
        vector<SDL_Gamepad *> m_gamepads;
    };
}

