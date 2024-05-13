# SDGL

A simple, direct game library

- 2D: sprites, bitmap fonts, texture atlases, camera
- FMOD sound library integration
- Cross-platform on Mac, Windows, Linux and HTML5

## Motivation

To build a game engine as a challenge to learn graphics programming.
I chose OpenGL because it's low-level enough to work with the graphics card (shaders, buffers),
and ANGLE makes compiling cross-platform with one codebase relatively easy.

ANGLE also gives a performance boost as a translation layer to native graphics libraries (Metal, Vulkan, D3D11).
And as the basis for WebGL in Chrome, it appears to be a reasonably steadfast technology.

## Technologies

- C++
- CMake
- OpenGL ES3 (ANGLE)
- Emscripten (web compiler)
- SDL2, FMOD, ImGui, glm, spdlog

## Licenses

| Library | License             |
|---------|---------------------|
| ANGLE   | BSD                 |
| imgui   | MIT                 |
| glaze   | MIT                 |
| glm     | MIT                 |
| SDL2    | ZLIB                |
| spdlog  | MIT                 |
| FMOD    | All rights reserved |

This repository's code is released as MIT, but please note that in order to use FMOD, you'll need to get a license from them and download their libraries from their website.

### Future

- 3D features
- Mobile, console platforms
- SDL3 backend
