# angle
set(ANGLES_SYSTEM_PROCESSOR ${SDGL_SYSTEM_PROCESSOR} CACHE STRING "" FORCE)
add_subdirectory(angles)

# glaze
add_subdirectory(glaze)

if (NOT EMSCRIPTEN)
    set(SDL_TEST OFF CACHE BOOL "" FORCE)
    set(SDL2_DISABLE_UNINSTALL ON CACHE BOOL "" FORCE)
    set(SDL_STATIC ON CACHE BOOL "" FORCE)
    set(SDL_SHARED OFF CACHE BOOL "" FORCE)
    add_subdirectory(SDL2)
endif()

set(FMOD_VERSION 2.02.21 CACHE STRING "" FORCE)
add_subdirectory(fmod-cmake)

# glm
set(GLM_ENABLE_CXX_20    ON  CACHE BOOL "Turn on C++ 20 for GLM" FORCE)
set(GLM_ENABLE_FAST_MATH ON  CACHE BOOL "Enable fast math optimizations" FORCE)
set(GLM_BUILD_LIBRARY    ON  CACHE BOOL "Build glm" FORCE)
set(GLM_BUILD_INSTALL    OFF CACHE BOOL "Don't install glm" FORCE)
add_subdirectory(glm)
target_include_directories(glm PUBLIC glm)

# ImGui
include(imgui.cmake)

# spdlog
set(SPDLOG_BUILD_TESTS  OFF CACHE BOOL "" FORCE)
if (APPLE)
    if (CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL "15.0.0")
        set(SPDLOG_USE_STD_FORMAT ON CACHE BOOL "" FORCE)
    endif()
else()
    set(SPDLOG_USE_STD_FORMAT ON CACHE BOOL "" FORCE)
endif()
add_subdirectory(spdlog)

if (EMSCRIPTEN)
    target_compile_options(spdlog PRIVATE -pthread)
    target_link_options(spdlog PRIVATE -pthread)
endif()


include(SDL_GameControllerDB.cmake)
include(stb.cmake)
