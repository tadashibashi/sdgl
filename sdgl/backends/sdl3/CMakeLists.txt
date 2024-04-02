
add_library(sdgl_backend STATIC
        ${CMAKE_SOURCE_DIR}/vendored/imgui/backends/imgui_impl_sdl3.cpp
        ${CMAKE_SOURCE_DIR}/vendored/imgui/backends/imgui_impl_sdl3.h
        ${CMAKE_SOURCE_DIR}/vendored/imgui/backends/imgui_impl_opengl3.cpp
        Sdl3Backend.h
        Sdl3Backend.cpp
        Sdl3Window.cpp
        Sdl3Window.h
        ImGuiSdl3.cpp
        ImGuiSdl3.h
)

# Linker flags
if (EMSCRIPTEN)
    target_link_options(sdgl_backend PUBLIC
            -sASSERTIONS=1 -sWASM=1 -sGL_ENABLE_GET_PROC_ADDRESS=1 -sFULL_ES3 -sMIN_WEBGL_VERSION=2 -sMAX_WEBGL_VERSION=2
            )
    #target_link_libraries(sdgl_backend PUBLIC ${sdgl_backend_LIBS})
    #target_include_directories(sdgl_backend PUBLIC ${CMAKE_SOURCE_DIR}/vendored/SDL/include)
else()
    #target_link_libraries(sdgl_backend PUBLIC ${sdgl_backend_LIBS} SDL3::SDL3)
endif()

target_link_libraries(sdgl_backend PUBLIC ${sdgl_backend_LIBS} SDL3::SDL3)
target_include_directories(sdgl_backend PRIVATE ${CMAKE_SOURCE_DIR})
target_compile_definitions(sdgl_backend PRIVATE -DIMGUI_IMPL_OPENGL_ES3=1)