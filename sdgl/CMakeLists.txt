add_subdirectory(backends)

add_library(sdgl STATIC
        sdglib.h
        Shader.h
        Shader.cpp
        backends/Backend.h
        Key.h
        Key.cpp
        ServiceContainer.cpp
        App.h
        Window.h
        Mouse.h
        PluginManager.cpp
        PluginManager.h
        IPlugin.h
        Window.cpp
        logging.h
        logging.cpp
        io/io.cpp
        io/io.h
        App.cpp
        Gamepad.h
        math/mathf.cpp
        sdgl_traits.h
        math/Vector2.h
        Tween.h
        Tween.cpp
        math/TweenFunction.h
        math/TweenFunction.cpp
        ecs/SystemManager.cpp
        ecs/SystemManager.h
        Camera2D.cpp
        Camera2D.h
        defaultShader.cpp
        defaultShader.h
        SpriteBatch.cpp
        SpriteBatch.h
        Color.h
        Color.cpp
        Texture2D.h
        Texture2D.cpp
        io/stb_image_impl.cpp
        platform/emscripten.h
        platform/emscripten.cpp
        Delegate.h
        ShaderAttribs.cpp
        ShaderAttribs.h
        graphics/RenderProgram.cpp
        graphics/RenderProgram.h
)

target_link_libraries(sdgl PRIVATE sdgl_backend PUBLIC glm::glm imgui spdlog::spdlog stb EnTT::EnTT)
target_include_directories(sdgl PUBLIC ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/vendored/angle/include
)
target_precompile_headers(sdgl PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/private/sdgl.pch)
string(TOUPPER "${CMAKE_BUILD_TYPE}" SDGL_BUILD_TYPE)
if ("${SDGL_BUILD_TYPE}" MATCHES "DEBUG")
    target_compile_definitions(sdgl PUBLIC -DSDGL_DEBUG=1)
endif()
if (EMSCRIPTEN)
    target_link_options(sdgl PUBLIC --preload-file ${CMAKE_SOURCE_DIR}/assets@/)
endif()


