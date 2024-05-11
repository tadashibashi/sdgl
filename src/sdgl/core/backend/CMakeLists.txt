# Link libraries
set(backend_LIBS imgui angles)

if ("${SDGL_BACKEND}" STREQUAL "sdl2")
    if (EMSCRIPTEN)
        set(backend_LINK_FLAGS
            -sUSE_SDL=2
            -sENVIRONMENT=web
            -sALLOW_MEMORY_GROWTH=1
            -sMODULARIZE=1
            -sEXPORT_NAME=SdglAppModule
            # -fwasm-exceptions # do NOT set: for some reason wasm-exceptions throws mysterious out-of-bounds error
            -sASSERTIONS=1 # do NOT set to 2: an exception will be thrown for some reason due to SDL2 input events unions' variant types
        )
        set(backend_FLAGS
            -sUSE_SDL=2
            # -fwasm-exceptions # do NOT set: for some reason wasm-exceptions throws mysterious out-of-bounds error
        )
    else()
        set(backend_LIBS ${backend_LIBS} SDL2::SDL2-static SDL2::SDL2main)
    endif()

    set(backend_SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/sdl2/ImGuiSdl2.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/sdl2/ImGuiSdl2.h
        ${CMAKE_CURRENT_SOURCE_DIR}/sdl2/Keyboard.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/sdl2/Gamepad.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/sdl2/Mouse.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/sdl2/SdlGuard.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/sdl2/Backend.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/sdl2/Window.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/sdl2/SdlGuard.cpp

        ${SDGL_ROOT_DIR}/lib/imgui/backends/imgui_impl_opengl3.cpp
        ${SDGL_ROOT_DIR}/lib/imgui/backends/imgui_impl_sdl2.cpp
    )

    set(backend_INCLUDES ${SDGL_ROOT_DIR}/lib/SDL2/include)
elseif("${SDGL_BACKEND}" STREQUAL "sdl3") # currently unsupported
    add_subdirectory(sdl3)
    set(backend_LIBS ${sdgl_backend_LIBS} SDL3::SDL3-static)
else()
    message(FATAL_ERROR "Backend not recognized")
endif()

set(sdgl_backend_LIBS ${backend_LIBS} CACHE STRING "" FORCE)
set(sdgl_backend_SRC ${backend_SRC} CACHE STRING "" FORCE)
set(sdgl_backend_LINK_FLAGS ${backend_LINK_FLAGS} CACHE STRING "" FORCE)
set(sdgl_backend_FLAGS ${backend_FLAGS} CACHE STRING "" FORCE)
set(sdgl_backend_INCLUDES ${backend_INCLUDES} CACHE STRING "" FORCE)
