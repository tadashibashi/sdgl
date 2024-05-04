add_subdirectory(core/backend)

add_library(sdgl STATIC
        sdglib.h
        graphics/Shader.h
        graphics/Shader.cpp
        core/backend/Backend.h
        core/Key.h
        core/Key.cpp
        ServiceContainer.cpp
        App.h
        core/Window.h
        core/Mouse.h
        core/PluginManager.cpp
        core/PluginManager.h
        core/IPlugin.h
        core/InputManager.h core/InputManager.cpp
        BitFlags.h BitFlags.cpp
        logging.h
        logging.cpp
        io/io.cpp
        io/io.h
        App.cpp
        core/GamepadConstants.h
        math/mathf.cpp
        sdgl_traits.h
        math/Vector2.h
        Tween.h
        Tween.cpp
        math/easings.h
        math/easings.cpp
        Camera2D.cpp
        Camera2D.h
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
        graphics/spriteBatch2DShader.inl
        math/random.cpp
        math/random.h
        math/Circle.h math/Circle.cpp
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
        io/convert/content.cpp
        io/convert/content.h
        io/Buffer.cpp
        io/Buffer.h
        SceneRunner.cpp
        SceneRunner.h
        Scene.cpp
        Scene.h
        core/GamepadConstants.cpp
        ContentManager.cpp
        ContentManager.h
        Asset.h
        graphics/Frame.h
        ArgParser.cpp
        ArgParser.h
        io/FileWriter.h
        io/FileWriter.cpp

        ${sdgl_backend_SRC}
        core/Gamepad.h
)

target_link_libraries(sdgl PUBLIC ${sdgl_backend_LIBS} glm::glm imgui spdlog::spdlog stb glaze::glaze)

target_include_directories(sdgl PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../ ${SDGL_ROOT_DIR}/include)
target_precompile_headers(sdgl PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/private/sdgl.pch)
string(TOUPPER "${CMAKE_BUILD_TYPE}" SDGL_BUILD_TYPE)
if ("${SDGL_BUILD_TYPE}" MATCHES "DEBUG" OR "${SDGL_BUILD_TYPE}" MATCHES "RELWITHDEBINFO")
    target_compile_definitions(sdgl PUBLIC -DSDGL_DEBUG=1)
else()
    target_compile_definitions(sdgl PUBLIC -DSDGL_DEBUG=0)
endif()

target_compile_definitions(sdgl PRIVATE -DIMGUI_IMPL_OPENGL_ES3=1)
