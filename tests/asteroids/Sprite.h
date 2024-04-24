#pragma once
#include <sdgl/sdglib.h>
#include <sdgl/graphics/atlas/TextureAtlas.h>

using namespace sdgl;

struct Sprite
{
    vector<TextureAtlas::Frame> frames;
    float framesPerSecond;
};

struct SpriteRenderer
{
    Sprite *sprite {};

    float speed = 1.f;
    float index = 0;

    void setSprite(Sprite *spr)
    {
        sprite = spr;
        index = 0;
    }
};
