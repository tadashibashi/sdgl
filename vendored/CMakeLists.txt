# Google Angle
if (NOT EMSCRIPTEN)
    add_subdirectory(angle)
endif()

# Catch2
if (SDGL_BUILD_TESTS)
    add_subdirectory(Catch2)
endif()

# GLFW
if (NOT EMSCRIPTEN AND "${SDGL_BACKEND}" STREQUAL "glfw3")
    set(GLFW_INSTALL        OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
    add_subdirectory(glfw)
endif()

# SDL3
if ("${SDGL_BACKEND}" STREQUAL "sdl3")
    set(SDL_TEST_LIBRARY     OFF CACHE BOOL "" FORCE)
    add_subdirectory(SDL)
endif()

# GLM
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

include(stb.cmake)

add_subdirectory(entt)
