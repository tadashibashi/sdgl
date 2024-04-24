#pragma once
#include <sdgl/App.h>
#include <sdgl/Camera2D.h>
#include <sdgl/ContentManager.h>
#include <sdgl/SceneRunner.h>
#include <sdgl/graphics/SpriteBatch2D.h>

#include <sdgl/graphics/gles3/gl.h>

using namespace sdgl;

class Asteroids final : public App
{
public:
    Asteroids()
        : App("Asteroids", 400, 400, WindowFlags::None, PluginConfig{.imgui=true})
    {
    }

    ~Asteroids() override = default;

protected:
    bool init() override
    {
        textures = content.loadTextureAtlas("atlas/sprites.bin");
        SDGL_ASSERT(textures, "Texture atlas loaded successfully");

        int winWidth, winHeight;
        getWindow()->getSize(&winWidth, &winHeight);
        camera.setViewport({0, 0, winWidth, winHeight});
        camera.setOrigin({0, 0});

        spriteBatch.init();

        scenes.services()
            .provide(&content)
            .provide(&spriteBatch);
        return true;
    }

    void update() override
    {
        const auto deltaTime = getDeltaTime();
        scenes.update(deltaTime);
    }

    void render() override
    {
        getWindow()->clear(Color{20, 20, 20, 255});

        const auto deltaTime = getDeltaTime();
        scenes.render(deltaTime);

        spriteBatch.begin(camera.getMatrix());

        spriteBatch.drawFrame(textures->at("rock/small/1"), {200, 200}, Color::Orange, {1, 1}, {8, 8}, mathf::toRadians(90), 0);

        spriteBatch.end();
    }

    void shutdown() override
    {
        content.unloadAll();
    }

private:
    SceneRunner scenes{};
    SpriteBatch2D spriteBatch{};
    ContentManager content{};
    Camera2D camera{};

    TextureAtlas *textures;
};