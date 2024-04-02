cmake_minimum_required(VERSION 3.26)

if (APPLE AND NOT EMSCRIPTEN)
    project(sdgl C CXX OBJC)
else()
    project(sdgl C CXX)
endif()

option(  SDGL_BUILD_TESTS   "Build unit tests" OFF)
set(     SDGL_BACKEND       "glfw3" CACHE STRING "Windowing backend library" FORCE) # glfw3, sdl3

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
set(CMAKE_CXX_STANDARD 20)

add_subdirectory(vendored)
add_subdirectory(sdgl)

if (SDGL_BUILD_TESTS)
    add_subdirectory(tests)
endif()
