set(sdgl_backend_SRC
    ${CMAKE_SOURCE_DIR}/vendored/imgui/backends/imgui_impl_sdl3.cpp
    ${CMAKE_SOURCE_DIR}/vendored/imgui/backends/imgui_impl_sdl3.h
    ${CMAKE_SOURCE_DIR}/vendored/imgui/backends/imgui_impl_opengl3.cpp
    Sdl3Backend.h
    Sdl3Backend.cpp
    Sdl3Window.cpp
    Sdl3Window.h
    ImGuiSdl3.cpp
    ImGuiSdl3.h

    CACHE STRING "" FORCE
)
