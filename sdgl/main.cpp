#include "backends/Backend.h"

#include <sdgl/gl.h>

#include <imgui.h>
#include <iostream>

#include "App.h"
#include "Texture2D.h"


using namespace sdgl;

class TestApp final : public App {
public:
    TestApp() : App("Test App", 640, 480, WindowFlags::Resizable | WindowFlags::Transparent)
    {

    }

protected:
    void init() override
    {
        m_texture.loadFile("RPG_interior.png");
    }

    void update() override
    {
        ImGui::ShowDemoWindow();

        if (ImGui::Begin("Test window"))
        {
            ImGui::Image((ImTextureID)m_texture.id(), {(float)m_texture.size().x * 2, (float)m_texture.size().y * 2});

        }
        ImGui::End();

        const auto window = getWindow();

        if (window->isPressed(Key::A))
        {
            std::cout << "I pressed A!\n";
        }

        if (window->isPressed(MouseBtn::Left))
        {
            std::cout << "Pressed left button\n";
        }

        if (window->isDown(MouseBtn::Right))
        {
            double x, y;
            window->getMousePosition(&x, &y);
            std::cout << "Right button is down at: " << x << ", " << y << '\n';
        }

        if (window->isReleased(Key::H))
        {
            std::cout << (window->isHovered() ? "Mouse hovering window" : "Not hovering") << '\n';
        }

        if (window->isPressed(0, GamepadBtn::A))
        {
            std::cout << "A was pressed\n";
        }

        const auto scrollY = window->getAxis(MouseAxis::ScrollY);
        if (scrollY != 0)
        {
            std::cout << "ScrollY: " << scrollY;
        }

        if (const auto axisLeft = window->getAxis(0, GamepadAxis::LeftX);
            abs(axisLeft) > .1f)
        {
            std::cout << "Left X axis stick: " << axisLeft << '\n';
        }

        if (window->isPressed(Key::Escape))
        {
            window->setShouldClose(true);
        }
    }

    void render() override
    {
        glClearColor(.5, .5, .5, .9);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void shutdown() override
    {
        m_texture.free();
    }

private:
    Texture2D m_texture;
};

int main()
{
    TestApp().run();

    return 0;
}