cmake_minimum_required(VERSION 3.26)

if (APPLE AND NOT EMSCRIPTEN)
    project(sdgl C CXX OBJC)
else()
    project(sdgl C CXX)
endif()

option(  SDGL_BUILD_TESTS   "Build unit tests" ON)
set(     SDGL_BACKEND       "glfw3" CACHE STRING "Windowing backend library") # glfw3, sdl3
set(     SDGL_GRAPHICS_LIB  "gles3" CACHE STRING "Graphics library backend")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
include(copy_assets)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(vendored)
add_subdirectory(sdgl)
add_subdirectory(tools)

if (SDGL_BUILD_TESTS)
    add_subdirectory(tests)
endif()
