add_subdirectory(core/backend)

add_library(sdgl STATIC
        audio/AudioEngine.h
        audio/AudioEngine.cpp
        audio/Sound.h
        audio/Sound.cpp
        audio/fmod.h

        core/backend/Backend.h
        core/Gamepad.h
        core/GamepadConstants.h
        core/GamepadConstants.cpp
        core/Key.h
        core/Key.cpp
        core/IPlugin.h
        core/InputManager.h core/InputManager.cpp
        core/Mouse.h
        core/PluginManager.cpp
        core/PluginManager.h
        core/Window.h

        graphics/atlas/TextureAtlas.cpp
        graphics/atlas/TextureAtlas.h
        graphics/atlas/CrunchAtlasData.cpp
        graphics/atlas/CrunchAtlasData.h
        graphics/Color.h
        graphics/Color.cpp
        graphics/font/BMFontData.cpp
        graphics/font/BMFontData.h
        graphics/font/BitmapFont.cpp
        graphics/font/BitmapFont.h
        graphics/font/Glyph.h
        graphics/font/FontText.cpp
        graphics/font/FontText.h
        graphics/Frame.h
        graphics/RenderProgram.cpp
        graphics/RenderProgram.h
        graphics/Shader.h
        graphics/Shader.cpp
        graphics/ShaderAttribs.cpp
        graphics/ShaderAttribs.h
        graphics/SpriteBatch2D.cpp
        graphics/SpriteBatch2D.h
        graphics/spriteBatch2DShader.inl
        graphics/Texture2D.h
        graphics/Texture2D.cpp
        graphics/TextureCache.cpp
        graphics/TextureCache.h

        io/Buffer.cpp
        io/Buffer.h
        io/BufferView.cpp
        io/BufferView.h
        io/convert/content.cpp
        io/convert/content.h
        io/FileWriter.h
        io/FileWriter.cpp
        io/io.cpp
        io/io.h
        io/stb_image_impl.cpp

        math/random.cpp
        math/random.h
        math/Circle.h
        math/Circle.cpp
        math/mathf.cpp
        math/Vector2.h
        math/easings.h
        math/easings.cpp

        platform/emscripten.h
        platform/emscripten.cpp

        App.h
        App.cpp
        ArgParser.cpp
        ArgParser.h
        Asset.h
        BitFlags.h
        BitFlags.cpp
        Camera2D.cpp
        Camera2D.h
        ContentManager.cpp
        ContentManager.h
        Delegate.h
        logging.h
        logging.cpp
        sdglib.h
        sdgl_traits.h
        SceneRunner.cpp
        SceneRunner.h
        Scene.cpp
        Scene.h
        ServiceContainer.h
        ServiceContainer.cpp
        Tween.h
        Tween.cpp

        ${sdgl_backend_SRC}
)

target_link_libraries(sdgl PUBLIC ${sdgl_backend_LIBS} glm::glm imgui spdlog::spdlog stb glaze::glaze fmod)

target_include_directories(sdgl PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../ ${SDGL_ROOT_DIR}/include ${sdgl_backend_INCLUDES})
target_precompile_headers(sdgl PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/private/sdgl.pch)
string(TOUPPER "${CMAKE_BUILD_TYPE}" SDGL_BUILD_TYPE)
if ("${SDGL_BUILD_TYPE}" MATCHES "DEBUG" OR "${SDGL_BUILD_TYPE}" MATCHES "RELWITHDEBINFO")
    target_compile_definitions(sdgl PUBLIC -DSDGL_DEBUG=1)
else()
    target_compile_definitions(sdgl PUBLIC -DSDGL_DEBUG=0)
endif()

target_compile_definitions(sdgl PRIVATE -DIMGUI_IMPL_OPENGL_ES3=1)
target_compile_options(sdgl PUBLIC ${sdgl_backend_FLAGS})
target_link_options(sdgl PUBLIC ${sdgl_backend_LINK_FLAGS})
