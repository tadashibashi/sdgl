#include <sdgl/gl.h>

#include <imgui.h>

#include <sdgl/App.h>
#include <sdgl/logging.h>
#include <sdgl/Texture2D.h>

#include <sdgl/Shader.h>
#include <sdgl/gl.h>

#include <sdgl/Camera2D.h>

using namespace sdgl;

class TestApp final : public App {
public:
    TestApp() : App("Test App", 640, 480, WindowFlags::Resizable | WindowFlags::Transparent)
    {

    }
private:
    Texture2D m_texture;
    Shader m_shader{};
    int u_texture, u_projMtx;
    uint m_vbo_positions, m_ebo_indices, m_vao, m_vbo_uv;

    Camera2D m_camera{};

protected:
    void init() override
    {
        m_texture.loadFile("RPG_interior.png");
        m_shader.load(
R"(#version 300 es

layout (location = 0) in vec4 Position;
layout (location = 1) in vec2 TextureUV;

out vec2 frag_TextureUV;

uniform mat4 ProjMtx;

void main()
{
    gl_Position = ProjMtx * Position;
    frag_TextureUV = TextureUV;
})",

R"(#version 300 es
precision mediump float;

in vec2 frag_TextureUV;

out vec4 out_Color;

uniform sampler2D u_texture;

void main()
{
    out_Color = texture(u_texture, frag_TextureUV);
})"
        );

        u_texture = m_shader.getLocation("u_texture");
        u_projMtx = m_shader.getLocation("ProjMtx");

        DEBUG_LOG("texture uniform id: {}", u_texture);
        DEBUG_LOG("projection matrix uniform id: {}", u_projMtx);

        glGenVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);

        auto texSize = Vector2(m_texture.size());
        const float vertices[] = {
            0, 0, 0, 1,
            0, texSize.y, 0, 1,
            texSize.x, texSize.y, 0, 1,
            texSize.x, 0, 0, 1,
        };

        constexpr float uv[] = {
            0, 0,
            0, 1,
            1, 1,
            1, 0,
        };

        constexpr uint indices[] = {
            0, 1, 2,
            0, 2, 3
        };

        glGenBuffers(1, &m_vbo_positions);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo_positions);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)nullptr);
        glEnableVertexAttribArray(0);

        glGenBuffers(1, &m_vbo_uv);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo_uv);
        glBufferData(GL_ARRAY_BUFFER, sizeof(uv), uv, GL_STATIC_DRAW);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)nullptr);
        glEnableVertexAttribArray(1);

        glGenBuffers(1, &m_ebo_indices);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo_indices);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glBindVertexArray(0);

        int wWidth, wHeight;
        getWindow()->getSize(&wWidth, &wHeight);
        m_camera
            .setViewport({0, 0, wWidth, wHeight})
            .setOrigin({.5, .5});
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

        if (window->isDown(Key::Left))
        {
            m_camera.setPosition({m_camera.getPosition().x - 4, m_camera.getPosition().y});
        }

        if (window->isDown(Key::Right))
        {
            m_camera.setPosition({m_camera.getPosition().x + 4, m_camera.getPosition().y});
        }

        if (window->isDown(Key::Up))
        {
            m_camera.setPosition({m_camera.getPosition().x, m_camera.getPosition().y - 4});
        }

        if (window->isDown(Key::Down))
        {
            m_camera.setPosition({m_camera.getPosition().x, m_camera.getPosition().y + 4});
        }

        if (window->isDown(Key::X))
        {
            m_camera.setScale({m_camera.getScale().x * 1.1f, m_camera.getScale().y * 1.1f});
        }


        if (window->isDown(Key::Z))
        {
            m_camera.setScale({m_camera.getScale().x * 0.9f, m_camera.getScale().y * 0.9f});
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

        int x, y;
        window->getFrameBufferSize(&x, &y);
        glViewport(0, 0, x, y);
    }

    void render() override
    {
        glClearColor(.5, .5, .9, .9);
        glClear(GL_COLOR_BUFFER_BIT);

        m_shader.use();
        glBindVertexArray(m_vao);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_texture.id());
        glUniform1i(u_texture, 0);

        glUniformMatrix4fv(u_projMtx, 1, false, m_camera.getMatrix());

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        glBindVertexArray(0);
    }

    void shutdown() override
    {
        glDeleteBuffers(1, &m_ebo_indices);
        glDeleteBuffers(1, &m_vbo_uv);
        glDeleteBuffers(1, &m_vbo_positions);
        glDeleteBuffers(1, &m_vao);
        m_texture.free();
    }


};

int main()
{
    TestApp().run();

    return 0;
}
