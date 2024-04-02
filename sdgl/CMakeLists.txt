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
        mathf.cpp
        sdgl_traits.h
        Vector2.h
        Tween.h
        Tween.cpp
        TweenFunction.h
        TweenFunction.cpp
)

target_link_libraries(sdgl PRIVATE sdgl_backend PUBLIC glm imgui spdlog::spdlog stb EnTT::EnTT)
target_include_directories(sdgl PUBLIC ${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/vendored/angle/include)
target_precompile_headers(sdgl PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/private/sdgl.pch)
string(TOUPPER "${CMAKE_BUILD_TYPE}" SDGL_BUILD_TYPE)
if ("${SDGL_BUILD_TYPE}" MATCHES "DEBUG")
    target_compile_definitions(sdgl PUBLIC -DSDGL_DEBUG=1)
endif()

if (EMSCRIPTEN)
    set(CMAKE_EXECUTABLE_SUFFIX .html)
endif()
project(sdgl_sandbox)
add_executable(sdgl_sandbox main.cpp)
target_link_libraries(sdgl_sandbox PRIVATE sdgl)
if (NOT EMSCRIPTEN)
    angle_copy_libs()
endif()
