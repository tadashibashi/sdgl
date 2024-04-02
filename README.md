# SDGL

A simple, direct game library

- Cross-platform on desktop and web
- Mobile not supported yet
- Tweens
- Entity / Components

## Technologies

- C++ 20
- OpenGL ES3 / Angle (Desktop builds)
- Emscripten (Web builds)

## Motivation

Build a cross-platform graphics library that is low-level enough to work with the
graphics card (shaders, buffers), but high level enough to avoid writing adaptations
for multiple modern graphics APIs.

ANGLE does the heavy lifting with regard to the adaptation layer. As the basis for WebGL 
in Chrome, it appears to be a steadfast option for OpenGL ES apps.

## Feature Roadmap

- [ ] Windowing backend (glfw)
- [ ] 2D graphic rendering
  - [ ] Shaders
  - [ ] Sprite atlas parsing & rendering
  - [ ] TTF / Bitmap font rendering
- [ ] Tiled TMX support


## Runtime Dependencies

| Library | License |
|---------|---------|
| ANGLE   | BSD     |
| imgui   | MIT     |
| glad    | MIT     |
| glaze   | MIT     |
| glm     | MIT     |
| glfw    | ZLIB    |
| spdlog  | MIT     |

### Future

- 3D features
- mobile, console platforms
- SDL3 backend
