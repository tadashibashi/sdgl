#pragma once
#include <sdgl/graphics/Color.h>

#include "Gamepad.h"
#include "Mouse.h"
#include "Key.h"
#include "PluginManager.h"
#include "../sdglib.h"

namespace sdgl {
    struct PluginConfig {
        /** Whether to include ImGui renderer - when true, ImGui functions may be called during update */
        bool imgui = true;
        /** User-provided plugins */
        vector<IPlugin *> plugins = {};
    };

    struct WindowFlags {
        enum Enum : uint {
            None = 0,
            Fullscreen = 1,
            Resizable = 1 << 1,
            AlwaysOnTop = 1 << 2,
            Borderless = 1 << 3,
            Transparent = 1 << 4,
        };
    };

    inline WindowFlags::Enum operator| (const WindowFlags::Enum a, const WindowFlags::Enum b)
    {
        return static_cast<WindowFlags::Enum>(static_cast<uint>(a) | static_cast<uint>(b));
    }

    class Window {
    public:
        virtual ~Window() = default;

        virtual Window &setSize(int x, int y) = 0;
        virtual void getSize(int *x, int *y) const = 0;

        virtual void getFrameBufferSize(int *x, int *y) = 0;

        virtual Window &setPosition(int x, int y) = 0;
        virtual void getPosition(int *x, int *y) = 0;

        /**
         * @return whether cursor is visible when inside this window
         */
        [[nodiscard]]
        virtual bool isCursorVisible() const = 0;
        virtual Window &setCursorVisible(bool value) = 0;

        [[nodiscard]]
        virtual bool shouldClose() const = 0;
        virtual Window &setShouldClose(bool value) = 0;

        [[nodiscard]]
        virtual bool isHidden() const = 0;
        virtual Window &setHidden(bool value) = 0;

        /**
         * @return whether cursor is hovering window
         */
        [[nodiscard]]
        virtual bool isHovered() const = 0;

        [[nodiscard]]
        virtual bool isFullscreen() const = 0;
        virtual Window &setFullscreen(bool value) = 0;

        [[nodiscard]]
        virtual bool isBorderless() const = 0;
        virtual Window &setBorderless(bool value) = 0;

        [[nodiscard]]
        virtual bool isAlwaysOnTop() const = 0;
        virtual Window &setAlwaysOnTop(bool value) = 0;

        [[nodiscard]]
        virtual bool isResizable() const = 0;
        virtual Window &setResizable(bool value) = 0;

        [[nodiscard]]
        virtual string_view getTitle() const = 0;
        virtual Window &setTitle(const string &title) = 0;

        // ===== Window Input =====

        [[nodiscard]]
        virtual bool isDown(Key::Enum key) const = 0;

        [[nodiscard]]
        virtual bool isPressed(Key::Enum key) const = 0;

        [[nodiscard]]
        virtual bool isUp(Key::Enum key) const = 0;

        [[nodiscard]]
        virtual bool isReleased(Key::Enum key) const = 0;

        [[nodiscard]]
        virtual bool isDown(MouseBtn::Enum button) const = 0;

        [[nodiscard]]
        virtual bool isPressed(MouseBtn::Enum button) const = 0;

        [[nodiscard]]
        virtual bool isUp(MouseBtn::Enum button) const = 0;

        [[nodiscard]]
        virtual bool isReleased(MouseBtn::Enum button) const = 0;

        [[nodiscard]]
        virtual float getAxis(MouseAxis::Enum axis) const = 0;
        [[nodiscard]]
        virtual float getAxisLast(MouseAxis::Enum axis) const = 0;

        virtual void getMousePosition(double *x, double *y) const = 0;

        [[nodiscard]]
        virtual bool isGamepadConnected(int index) const = 0;
        [[nodiscard]]
        virtual bool isDown(int index, GamepadBtn::Enum button) const = 0;
        [[nodiscard]]
        virtual bool isPressed(int index, GamepadBtn::Enum button) const = 0;
        [[nodiscard]]
        virtual float getAxis(int index, GamepadAxis::Enum axis) const = 0;
        [[nodiscard]]
        virtual float getAxisLast(int index, GamepadAxis::Enum axis) const = 0;

        /**
         *
         * @return pointer to the backend window object
         */
        [[nodiscard]]
        virtual void *getHandle() const = 0;

        virtual void swapBuffers() const = 0;

        virtual void makeCurrent() const = 0;

        virtual void clear(Color color) = 0;

        /**
         * Start frame for plugins, makes this window context current
         */
        void startFrame();

        /**
         * End frame for plugins, makes this window context current
         */
        void endFrame();

        /**
         * Render plugins, makes this window context current
         */
        void render();

        PluginManager *plugins() { return &m_plugins; }

    private:
        PluginManager m_plugins;
    };
}
