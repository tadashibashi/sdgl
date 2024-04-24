# SDGL Core

This folder contains files directly pertaining to OS windowing and input
backend library implementations.

Each backend creates the `sdgl_backend` project, so only one backend subdirectory is 
included to the CMake project via `add_subdirectory` at a time.

## Backends
For now, only glfw3 is fully supported, and SDL3 implementation is currently 
left incomplete until the upstream has a full stable 3.2.0 release.
SDL3 may help expedite porting to other platforms, and eventually
it would be ideal to use their upcoming intermediate shading library
as a replacement for opengl es3 / angle

To help aid this potential shift, sdgl/graphics/gles3 folder contains all the
functions pertaining to OpenGL ES3, and leaves room for implementing these 
classes with another graphics library