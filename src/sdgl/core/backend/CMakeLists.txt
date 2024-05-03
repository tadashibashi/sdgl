# Link libraries
set(backend_LIBS imgui angles)

if ("${SDGL_BACKEND}" STREQUAL "sdl2")
    include(FetchContent)

    set(SDL_TEST OFF CACHE BOOL "" FORCE)
    set(SDL2_DISABLE_UNINSTALL ON CACHE BOOL "" FORCE)
    set(SDL_STATIC ON CACHE BOOL "" FORCE)
    set(SDL_DYNAMIC OFF CACHE BOOL "" FORCE)

    # Fetch SDL2 release 2.30.2
    FetchContent_Declare(SDL2
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG        fb1497566c5a05e2babdcf45ef0ab5c7cca2c4ae
    )

    FetchContent_MakeAvailable(SDL2)
    set(backend_LIBS ${backend_LIBS} SDL2::SDL2 SDL2::SDL2main)

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
elseif("${SDGL_BACKEND}" STREQUAL "sdl3") # currently unsupported
    add_subdirectory(sdl3)
    set(backend_LIBS ${sdgl_backend_LIBS} SDL3::SDL3)
else()
    message(FATAL_ERROR "Backend not recognized")
endif()

# add_library(sdgl_backend INTERFACE ${sdgl_backend_SRC})
# target_link_libraries(sdgl_backend INTERFACE ${sdgl_backend_LIBS})
set(sdgl_backend_LIBS ${backend_LIBS} CACHE STRING "" FORCE)
set(sdgl_backend_SRC ${backend_SRC} CACHE STRING "" FORCE)