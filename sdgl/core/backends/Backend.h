#pragma once

#include "../Window.h"

namespace sdgl {
    class IPlugin;
}

namespace sdgl::backends {

    /**
     * A backend is an implementation for a windowing library
     */
    class Backend {
    public:
        virtual ~Backend() = default;

        virtual bool init() = 0;

        /**
         * @return Time in seconds since the app started. Should be finer than milliseconds.
         */
        [[nodiscard]]
        virtual double getAppTime() const = 0;

        [[nodiscard]]
        virtual string_view name() const = 0;

        virtual Window *createWindow(const string &title, int width, int height, WindowFlags::Enum flags, const PluginConfig &plugins) = 0;
        virtual void destroyWindow(Window *window) = 0;

        virtual void processInput() = 0;

        virtual void shutdown() = 0;
    private:
        PluginConfig m_config;
    };

    /**
     * Implementation should implement this function
     */
    Backend *createBackend();
}