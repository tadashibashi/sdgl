#include <sdgl/gl.h>

#include <imgui.h>

#include <sdgl/App.h>

#include <sdgl/Camera2D.h>
#include <sdgl/graphics/SpriteBatch2D.h>

#include <sdgl/graphics/Color.h>

#include <sdgl/math/geometry.h>
#include <sdgl/math/random.h>
#include <sdgl/math/Rectangle.h>
#include <sdgl/graphics/Texture2D.h>
#include <sdgl/logging.h>
#include <sdgl/graphics/BitmapFont.h>
#include <sdgl/graphics/FontText.h>

using namespace sdgl;

struct Sprite
{
    Texture2D texture = {};
    Vector2 position = {};
    float rotation = 0;
    float rotationSpeed = 1.f;
    Vector2 speed = {};

    void update()
    {
        rotation += rotationSpeed;
        position += speed;
    }
};

class TestApp final : public App {
public:
    TestApp() : App("Test App", 640, 480, WindowFlags::Resizable | WindowFlags::Transparent)
    {

    }
private:
    Texture2D m_texture;

    graphics::SpriteBatch2D m_batch{};

    Camera2D m_camera{};
    vector<Sprite> m_sprites{};

    graphics::BitmapFont m_font{};
    graphics::FontText m_text{};
protected:
    bool init() override
    {
        m_texture.loadFile("RPG_interior.png");

        int wWidth, wHeight;
        getWindow()->getSize(&wWidth, &wHeight);
        m_camera
            .setViewport({0, 0, wWidth, wHeight})
            .setOrigin({.0, .0});

        m_batch.init();

        // for (int i = 0; i < 1000; ++i)
        // {
        //     m_sprites.emplace_back(Sprite{
        //         .position = {mathf::rand(wWidth), mathf::rand(wHeight)},
        //         .rotation = mathf::rand(360.f),
        //         .speed = {mathf::rand(2.f), mathf::rand(2.f)},
        //         .texture = m_texture,
        //         .rotationSpeed = mathf::rand(2.f) - 1.f
        //     });
        // }
        m_sprites.emplace_back(Sprite{
            .position = {0, 0},
            .rotation = 0,
            .speed = {},
            .texture = m_texture,
            .rotationSpeed = 0
        });

        SDGL_ASSERT(m_font.loadBMFont("assets/bmfont/font.fnt"), "Font should load without problems");
        m_text = graphics::FontText(&m_font, "Hello world!\nAnother World", 0, true, 0, 0);
        DEBUG_LOG(m_text.getText().length());

        return true;
    }

    const float textTimerMax = .05f;
    float textTimer = textTimerMax;
    float lastFrameTime = 0;

    void update() override
    {
        const auto curTime = getTime();
        auto deltaTime = curTime - lastFrameTime;

        textTimer -= deltaTime;
        if (textTimer <= 0)
        {
            textTimer += textTimerMax;
            m_text.textProgress((m_text.textProgress() + 1) % m_text.glyphs().size());
        }

        ImGui::ShowDemoWindow();

        if (ImGui::Begin("Test window"))
        {
            ImGui::Text(format("{}", m_camera.getWorldBounds()).c_str());
        }
        ImGui::End();

        const auto window = getWindow();

        if (window->isPressed(Key::A))
        {
            DEBUG_LOG("I pressed A!");
        }

        if (window->isDown(Key::Left))
        {
            m_camera.setPosition({m_camera.getPosition().x - 4.f, m_camera.getPosition().y});
        }

        if (window->isDown(Key::Right))
        {
            m_camera.setPosition({m_camera.getPosition().x + 4.f, m_camera.getPosition().y});
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

        if (window->isDown(0, GamepadBtn::Guide))
        {
            DEBUG_LOG("Gamepad guide down");
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
            m_camera.setRotation(m_camera.getRotation() - axisLeft * 10);
        }

        if (window->isPressed(Key::Escape))
        {
            window->setShouldClose(true);
        }

        for (auto &sprite : m_sprites)
        {
            sprite.update();
        }

        int x, y;
        window->getFrameBufferSize(&x, &y);
        glViewport(0, 0, x, y);

        lastFrameTime = curTime;
    }

    void render() override
    {
        getWindow()->makeCurrent();
        glClearColor(.5, .5, .9, .9);
        glClear(GL_COLOR_BUFFER_BIT);

        m_batch.begin(m_camera.getMatrix());
        auto worldBounds = m_camera.getWorldBounds();

        for (const auto &sprite : m_sprites)
        {
            // block non-visible textures from being drawn
            Vector2 anchor = {sprite.texture.size().x / 2.f, sprite.texture.size().y / 2.f};
            float largestSide = sprite.texture.size().x > sprite.texture.size().y ?
                sprite.texture.size().x : sprite.texture.size().y;
            FRectangle spriteZone { -largestSide / 2.f + sprite.position.x, -largestSide / 2.f + sprite.position.y,
                largestSide, largestSide };
            if (!mathf::intersects(spriteZone, worldBounds))
                continue;

            m_batch.drawTexture(sprite.texture,
                {0, 0, sprite.texture.size().x, sprite.texture.size().y},
                sprite.position,
                Color{}, {1, 1},
                anchor,
                sprite.rotation, 0);
        }

        int winWidth, winHeight;
        getWindow()->getSize(&winWidth, &winHeight);
        m_batch.drawText(m_text, {(float)winWidth / 2.f, (float)winHeight / 2.f});

        // m_batch.drawTexture(m_text.glyphs()[0].texture, {
        //     0, 0, m_text.glyphs()[0].texture.size().x, m_text.glyphs()[0].texture.size().y},
        //     {0, 0}, Color::White, {1.f, 1.f}, {0, 0}, 0, 0);
        m_batch.end();
    }

    void shutdown() override
    {
        m_texture.free();
        m_font.free();
    }


};

int main(int argc, char *argv[])
{
    return TestApp().run(argc, argv);
}
