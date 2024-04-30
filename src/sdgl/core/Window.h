#pragma once
#include "Gamepad.h"
#include "Mouse.h"
#include "Key.h"
#include "PluginManager.h"
#include "InputManager.h"

#include <sdgl/sdglib.h>
#include <sdgl/graphics/Color.h>

struct SDL_Window;
using SDL_GLContext = void *;

namespace sdgl {
    class Color;

    struct PluginConfig {
        /** Whether to include ImGui renderer - when true, ImGui functions may be called during update */
        bool imgui = true;
        /** User-provided plugins */
        vector<IPlugin *> plugins = {};
    };

    struct WindowInit {
        enum Flags : uint {
            None = 0,
            Fullscreen = 1,
            Resizable = 1 << 1,
            AlwaysOnTop = 1 << 2,
            Borderless = 1 << 3
        };
    };

    inline constexpr WindowInit::Flags operator| (const WindowInit::Flags a, const WindowInit::Flags b)
    {
        return static_cast<WindowInit::Flags>(static_cast<uint>(a) | static_cast<uint>(b));
    }
    inline constexpr WindowInit::Flags &operator|=(WindowInit::Flags &a, const WindowInit::Flags b)
    {
        return a = a | b;
    }

    class Window {
    public:
        Window(SDL_Window *window, SDL_GLContext context);
        ~Window();

        Window &setSize(int x, int y);
        void getSize(int *x, int *y) const;

        void getFrameBufferSize(int *x, int *y) const;

        Window &setPosition(int x, int y);
        void getPosition(int *x, int *y) const;

        [[nodiscard]]
        bool shouldClose() const;
        Window &setShouldClose(bool value);

        [[nodiscard]]
        bool isHidden() const;
        Window &setHidden(bool value);

        [[nodiscard]]
        bool isFullscreen() const;
        Window &setFullscreen(bool value);

        [[nodiscard]]
        bool isBorderless() const;
        Window &setBorderless(bool value);

        [[nodiscard]]
        bool isAlwaysOnTop() const;
        Window &setAlwaysOnTop(bool value);

        [[nodiscard]]
        bool isResizable() const;
        Window &setResizable(bool value);

        [[nodiscard]]
        string_view getTitle() const;
        Window &setTitle(const string &title);

        /**
         *
         * @return pointer to the backend window object
         */
        [[nodiscard]]
        void *getHandle() const;


        void swapBuffers() const;
        void makeCurrent() const;
        void clear(const Color &color);

        /**
         * Start frame for plugins, makes this window context current
         */
        void startFrame() { m_plugins.startFrame(); }

        /**
         * End frame for plugins, makes this window context current
         */
        void endFrame() { m_plugins.endFrame(); }

        /**
         * Render plugins, makes this window context current
         */
        void render() { m_plugins.render(); }

        PluginManager *plugins() { return &m_plugins; }

        InputManager *input();

    private:
        friend class Backend;
        PluginManager m_plugins;
        struct Impl;
        Impl *m;
    };
}
