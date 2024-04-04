//
// Created by Aaron Ishibashi on 4/2/24.
//

#include "defaultShader.h"

#include "Shader.h"

namespace sdgl {
    static const auto defaultVertexShader =
R"(#version 300 es
precision highp float;

layout (location = 0) in vec2 vertex_position;
layout (location = 1) in vec4 vertex_color;
layout (location = 2) in vec2 vertex_uv;

out vec2 fragment_position;
out vec4 fragment_color;
out vec2 fragment_uv;

uniform mat4 P;

void main() {
	gl_Position.xy = (P * vec4(vertex_position, 0.0, 1.0)).xy;
	gl_Position.z = 0.0;
	gl_Position.w = 1.0;

	fragment_position = vertex_position;
	fragment_color = vertex_color;

	fragment_uv = vec2(vertex_uv.x, 1.0 - vertex_uv.y);
}
)";

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

            s_defaultShader.addAttribute("vertex_position");
            s_defaultShader.addAttribute("vertex_color");
            s_defaultShader.addAttribute("vertex_uv");
        }

        return &s_defaultShader;
    }
}

