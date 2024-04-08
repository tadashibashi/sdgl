add_subdirectory(backends)

add_library(sdgl STATIC
        sdglib.h
        graphics/Shader.h
        graphics/Shader.cpp
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
        graphics/SpriteBatch2D.cpp
        graphics/SpriteBatch2D.h
        graphics/Color.h
        graphics/Color.cpp
        graphics/Texture2D.h
        graphics/Texture2D.cpp
        io/stb_image_impl.cpp
        platform/emscripten.h
        platform/emscripten.cpp
        Delegate.h
        graphics/ShaderAttribs.cpp
        graphics/ShaderAttribs.h
        graphics/RenderProgram.cpp
        graphics/RenderProgram.h
        graphics/TextureCache.cpp
        graphics/TextureCache.h
        graphics/detail/spriteBatch2DShader.inl
        math/random.cpp
        math/random.h
        graphics/font/BMFontData.cpp
        graphics/font/BMFontData.h
        io/BufferView.cpp
        io/BufferView.h
        graphics/font/BitmapFont.cpp
        graphics/font/BitmapFont.h
        graphics/font/Glyph.h
        graphics/font/FontText.cpp
        graphics/font/FontText.h
        graphics/atlas/TextureAtlas.cpp
        graphics/atlas/TextureAtlas.h
        graphics/atlas/CrunchAtlasData.cpp
        graphics/atlas/CrunchAtlasData.h
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


