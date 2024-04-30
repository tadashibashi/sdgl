#pragma once

#include "../Window.h"

struct SDL_WindowEvent;

namespace sdgl {
    class IPlugin;

    /**
     * A backend is an implementation for a windowing library
     */
    class Backend {
    public:
        Backend();
        ~Backend();

        bool init();

        /**
         * @return Time in seconds since the app started. Should be finer than milliseconds.
         */
        [[nodiscard]]
        double getAppTime() const;

        [[nodiscard]]
        string_view name() const;

        Window *createWindow(const string &title, int width, int height, WindowInit::Flags flags, const PluginConfig &plugins);
        void destroyWindow(Window *window);

        void processInput();

        void shutdown();
    private:
        void processWindowEvent(const SDL_WindowEvent &e);
        PluginConfig m_config;

        struct Impl;
        Impl *m;
    };
}