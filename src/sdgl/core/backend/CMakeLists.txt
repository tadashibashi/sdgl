# Link libraries
set(sdgl_backend_LIBS imgui angles)

if ("${SDGL_BACKEND}" STREQUAL "sdl2")
    include(FetchContent)

    # Fetch SDL2 release 2.30.2
    FetchContent_Declare(SDL2
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG        f461d91cd265d7b9a44b4d472b1df0c0ad2855a0
    )

    FetchContent_MakeAvailable(SDL2)
    list(APPEND sdgl_backend_LIBS SDL2::SDL2)

    set(sdgl_backend_SRC
        sdl2/ImGuiSdl2.cpp
        sdl2/ImGuiSdl2.h
        sdl2/Backend.cpp
        sdl2/Window.cpp
        sdl2/SdlGuard.cpp
    )
elseif("${SDGL_BACKEND}" STREQUAL "sdl3") # currently unsupported
    add_subdirectory(sdl3)
    list(APPEND sdgl_backend_LIBS SDL3::SDL3)
else()
    message(FATAL_ERROR "Backend not recognized")
endif()

add_library(sdgl_backend INTERFACE ${sdgl_backend_SRC})
target_link_libraries(sdgl_backend INTERFACE ${sdgl_backend_LIBS})
