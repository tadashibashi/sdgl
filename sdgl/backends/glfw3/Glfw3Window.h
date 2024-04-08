#pragma once
#include <GLFW/glfw3.h>
#include <sdgl/Window.h>

#include <utility>

#include "Glfw3Gamepad.h"
#include "Glfw3Keyboard.h"
#include "Glfw3Mouse.h"

namespace sdgl::backends::glfw3 {
    class Glfw3Window : public Window {
    public:
        explicit Glfw3Window(GLFWwindow *window, const vector<Glfw3Gamepad> &gamepads, string title) :
            m_window(window), m_mouse(), m_keyboard(), m_gamepads(gamepads), m_title(std::move(title))
        {
            m_mouse.init(window);
            m_keyboard.init(window);
        }

        ~Glfw3Window() override = default;

        Window &setSize(int width, int height) override;
        void getSize(int *x, int *y) const override;

        void getFrameBufferSize(int *x, int *y) override;

        Window &setPosition(int x, int y) override;
        void getPosition(int *x, int *y) override;

        /**
         * @return whether cursor is visible when inside this window
         */
        [[nodiscard]]
        bool isCursorVisible() const override;
        Window &setCursorVisible(bool value) override;

        /**
         * @return whether window has been requested to close by the user or os
         */
        [[nodiscard]]
        bool shouldClose() const override;
        Window &setShouldClose(bool value) override;

        [[nodiscard]]
        bool isHidden() const override;
        Window &setHidden(bool value) override;

        /**
         * @return whether cursor is hovering window
         */
        [[nodiscard]]
        bool isHovered() const override;

        [[nodiscard]]
        bool isFullscreen() const override;
        Window &setFullscreen(bool value) override;

        [[nodiscard]]
        bool isBorderless() const override;
        Window &setBorderless(bool value) override;

        [[nodiscard]]
        bool isAlwaysOnTop() const override;
        Window &setAlwaysOnTop(bool value) override;

        [[nodiscard]]
        bool isResizable() const override;
        Window &setResizable(bool value) override;

        [[nodiscard]]
        string_view getTitle() const override;
        Window &setTitle(const string &title) override;

        // ===== Window Input =====

        [[nodiscard]]
        bool isDown(Key::Enum key) const override;

        [[nodiscard]]
        bool isPressed(Key::Enum key) const override;

        [[nodiscard]]
        bool isUp(Key::Enum key) const override;

        [[nodiscard]]
        bool isReleased(Key::Enum key) const override;

        [[nodiscard]]
        bool isDown(MouseBtn::Enum button) const override;

        [[nodiscard]]
        bool isPressed(MouseBtn::Enum button) const override;

        [[nodiscard]]
        bool isUp(MouseBtn::Enum button) const override;

        [[nodiscard]]
        bool isReleased(MouseBtn::Enum button) const override;

        [[nodiscard]]
        float getAxis(MouseAxis::Enum axis) const override;
        [[nodiscard]]
        float getAxisLast(MouseAxis::Enum axis) const override;

        void getMousePosition(double *x, double *y) const override;

        [[nodiscard]]
        bool isGamepadConnected(int index) const override;
        [[nodiscard]]
        bool isDown(int index, GamepadBtn::Enum button) const override;
        [[nodiscard]]
        bool isPressed(int index, GamepadBtn::Enum button) const override;
        [[nodiscard]]
        float getAxis(int index, GamepadAxis::Enum axis) const override;
        [[nodiscard]]
        float getAxisLast(int index, GamepadAxis::Enum axis) const override;

        /// @return pointer to the backend window object
        [[nodiscard]]
        void *getHandle() const override;

        void swapBuffers() const override;

        void makeCurrent() const override;
    private:
        friend class Glfw3Mouse;
        friend class Glfw3Keyboard;
        friend class Glfw3Backend;
        void preProcessInput();
        void processInput();

        GLFWwindow *m_window;
        Glfw3Mouse m_mouse;
        Glfw3Keyboard m_keyboard;
        const vector<Glfw3Gamepad> &m_gamepads;
        string m_title;
    };
}
