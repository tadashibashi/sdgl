# Graphics library
add_subdirectory(angles)

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

include(SDL_GameControllerDB.cmake)

add_subdirectory(glaze)
