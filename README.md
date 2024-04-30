# SDGL

A simple, direct game library

- 2D features
- Cross-platform on desktop and web


## Technologies

- C++ 20
- OpenGL ES3 / Angle (desktop builds)
- Emscripten (web builds)
- SDL2 (windowing / input)

## Motivation

Build a game engine as a challenge to learn graphics programming.
I chose OpenGL because it's low-level enough to work with the graphics card (shaders, buffers),
and ANGLE makes compiling cross-platform as easy as adding a few conditional defines.

ANGLE also gives a performance boost as a translation layer to native graphics libraries (Metal, Vulkan, D3D11).
And as the basis for WebGL in Chrome, it appears to be a reasonably steadfast technology.

## Runtime Dependencies

| Library | License |
|---------|---------|
| ANGLE   | BSD     |
| imgui   | MIT     |
| glad    | MIT     |
| glaze   | MIT     |
| glm     | MIT     |
| SDL2    | ZLIB    |
| spdlog  | MIT     |

### Future

- 3D features
- mobile, console platforms
- SDL3 backend
