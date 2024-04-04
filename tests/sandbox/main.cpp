#include <sdgl/gl.h>

#include <imgui.h>

#include <sdgl/App.h>
#include <sdgl/logging.h>
#include <sdgl/Texture2D.h>

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
            ImGui::Image(reinterpret_cast<ImTextureID>(m_texture.id()),
                {
                    static_cast<float>(m_texture.size().x) * 2,
                    static_cast<float>(m_texture.size().y) * 2
                });
        }
        ImGui::End();

        const auto window = getWindow();

        if (window->isPressed(Key::A))
        {
            DEBUG_LOG("I pressed A!");
        }

        if (window->isPressed(MouseBtn::Left))
        {
            DEBUG_LOG("Pressed left button\n");
        }

        if (window->isDown(MouseBtn::Right))
        {
            double x, y;
            window->getMousePosition(&x, &y);
            DEBUG_LOG("Right button is down at: {}, {}", x, y);
        }

        if (window->isReleased(Key::H))
        {
            DEBUG_LOG(window->isHovered() ? "Mouse hovering window" : "Not hovering");
        }

        if (window->isPressed(0, GamepadBtn::A))
        {
            DEBUG_LOG("A was pressed");
        }

        const auto scrollY = window->getAxis(MouseAxis::ScrollY);
        if (scrollY != 0)
        {
            DEBUG_LOG("ScrollY: {}", scrollY);
        }

        if (const auto axisLeft = window->getAxis(0, GamepadAxis::LeftX);
            abs(axisLeft) > .1f)
        {
            DEBUG_LOG("Left X axis stick: {}", axisLeft);
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