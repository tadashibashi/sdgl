cmake_minimum_required(VERSION 3.26)

set(CMAKE_CXX_STANDARD 20)

if (APPLE AND NOT EMSCRIPTEN)
    project(sdgl C CXX OBJC)
else()
    project(sdgl C CXX)
endif()

set(     SDGL_BACKEND       "sdl2" CACHE STRING "Windowing backend library") # glfw3, sdl3
set(     SDGL_GRAPHICS_LIB  "gles3" CACHE STRING "Graphics library backend")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

set(SDGL_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE STRING "" FORCE)

add_subdirectory(lib)
add_subdirectory(src)

if (CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR OR SDGL_BUILD_TESTS)
    add_subdirectory(tests)
endif()
