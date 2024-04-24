#pragma once

namespace sdgl::detail {

    static const auto spriteBatch2dVertShader =
R"glsl(#version 300 es
layout (location = 0) in vec2 Position;
layout (location = 1) in vec2 TexCoord;
layout (location = 2) in vec4 Color;

out vec2 frag_TextureUV;
out vec4 frag_Color;

uniform mat4 u_ProjMtx;
uniform vec2 u_TexSize;

void main()
{
    gl_Position = u_ProjMtx * vec4(Position.x, Position.y, 0, 1.0);
    frag_TextureUV = TexCoord / u_TexSize;
    frag_Color = Color;
})glsl";

    static const auto spriteBatch2dFragShader =
R"glsl(#version 300 es
precision mediump float;

in vec2 frag_TextureUV;
in vec4 frag_Color;

out vec4 out_Color;

uniform sampler2D u_Texture;

void main()
{
    out_Color = texture(u_Texture, frag_TextureUV) * frag_Color;
})glsl";

}
