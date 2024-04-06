//
// Created by Aaron Ishibashi on 4/2/24.
//

#include "defaultShader.h"

#include "graphics/Shader.h"

namespace sdgl {
    static const auto defaultVertexShader =
R"(#version 300 es

layout (location = 0) in vec2 Position;
layout (location = 1) in vec2 TextureUV;

out vec2 frag_TextureUV;

uniform mat4 u_ProjMtx;

void main()
{
    gl_Position = u_ProjMtx * vec4(Position, 0, 1);
    frag_TextureUV = TextureUV;
})";

	static const auto defaultFragmentShader =
R"(#version 300 es
precision mediump float;

in vec2 fragment_position;
in vec4 fragment_color;
in vec2 fragment_uv;

layout (location = 0) out vec4 fragColor;

uniform sampler2D u_texture;
uniform ivec2 u_size;
float sharpness = 2.5;

float sharpen(float pix_coord) {
    float norm = (fract(pix_coord) - 0.5) * 2.0;
    float norm2 = norm * norm;
    return floor(pix_coord) + norm * pow(norm2, sharpness) / 2.0 + 0.5;
}

void main() {
    vec2 vres = vec2(u_size.x, u_size.y);
    fragColor = texture(u_texture, vec2(
        sharpen(fragment_uv.x * vres.x) / vres.x,
        sharpen(fragment_uv.y * vres.y) / vres.y
    )) * fragment_color;
}
)";
    static Shader s_defaultShader;

    const Shader *getDefaultShader()
    {
        if (!s_defaultShader.isLoaded())
        {
            if (!s_defaultShader.load(defaultVertexShader, defaultFragmentShader))
            {
                throw std::runtime_error("Failed to load default shader");
            }
        }

        return &s_defaultShader;
    }
}

