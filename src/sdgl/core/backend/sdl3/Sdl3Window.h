#pragma once

#include <../../Window.h>

#include <SDL3/SDL_video.h>

namespace sdgl::backends::sdl3 {
    class Sdl3Window final : public Window {
    public:
        explicit Sdl3Window(SDL_Window *window, SDL_GLContext context);

        Window &setSize(int width, int height) override;
        void getSize(int *width, int *height) const override;

        Window &setPosition(int x, int y) override;
        void getPosition(int *x, int *y) override;

        /**
         * @return whether cursor is visible when inside this window
         */
        [[nodiscard]]
        bool isCursorVisible() const override;
        Window &setCursorVisible(bool value) override;

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

        void getMousePosition(double *x, double *y) const override;

        [[nodiscard]]
        float getAxisLast(MouseAxis::Enum axis) const override;

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

        /**
         *
         * @return pointer to the backend window object
         */
        [[nodiscard]]
        void *getHandle() const override;

        void swapBuffers() const override;

        void makeCurrent() const override;



    private:
        friend class Sdl3Backend;
        SDL_Window *m_window;
        SDL_GLContext m_context;
        bool m_shouldClose;
    };
}
