# SDGL GLFW3 Backend

add_library(sdgl_backend STATIC
        ${CMAKE_SOURCE_DIR}/vendored/imgui/backends/imgui_impl_glfw.cpp
        ${CMAKE_SOURCE_DIR}/vendored/imgui/backends/imgui_impl_glfw.h
        ${CMAKE_SOURCE_DIR}/vendored/imgui/backends/imgui_impl_opengl3.cpp
        Glfw3ImGuiPlugin.cpp
        Glfw3ImGuiPlugin.h
        Glfw3Backend.cpp
        Glfw3Backend.h
        Glfw3Keyboard.h
        Glfw3Keyboard.cpp
        Glfw3Gamepad.h
        Glfw3Gamepad.cpp
        Glfw3Mouse.h
        Glfw3Mouse.cpp
        Glfw3Window.h
        Glfw3Window.cpp
)

# Linker flags
if (EMSCRIPTEN)
    target_link_options(sdgl_backend PUBLIC
            -sASSERTIONS=1 -sWASM=1 -sASYNCIFY --use-port=contrib.glfw3 -sFULL_ES3 -sMIN_WEBGL_VERSION=2 -sMAX_WEBGL_VERSION=2)
    target_include_directories(sdgl_backend PUBLIC ${CMAKE_SOURCE_DIR}/vendored/glfw/include)
    target_link_libraries(sdgl_backend PUBLIC ${sdgl_backend_LIBS})
else()
    target_link_libraries(sdgl_backend PUBLIC ${sdgl_backend_LIBS} glfw)
endif()



target_include_directories(sdgl_backend PRIVATE ${CMAKE_SOURCE_DIR})
target_compile_definitions(sdgl_backend PRIVATE -DIMGUI_IMPL_OPENGL_ES3=1)