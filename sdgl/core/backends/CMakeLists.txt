if (NOT SDGL_BACKEND)
    set(SDGL_BACKEND "glfw3")
endif()

# Link libraries
set(sdgl_backend_LIBS imgui)

if (NOT EMSCRIPTEN)
    list(APPEND sdgl_backend_LIBS angles)

else()
#    find_package(OpenGL REQUIRED)
#    list(APPEND sdgl_backend_LIBS $<TARGET_NAME_IF_EXISTS:OpenGL::GL>)
endif()

if ("${SDGL_BACKEND}" STREQUAL "glfw3")
    add_subdirectory(glfw3)
elseif("${SDGL_BACKEND}" STREQUAL "sdl3")
    add_subdirectory(sdl3)
else()
    message(FATAL_ERROR "Backend not recognized")
endif()

# Graphics library definition that backend will need to implement
# Current:
#   - OpenGL ES3 / Angle
# Future: SDL3 Shading / Graphics abstraction

if (NOT SDGL_GRAPHICS_LIB)
    set(SDGL_GRAPHICS_LIB "gles3")
endif()

if ("${SDGL_GRAPHICS_LIB}" STREQUAL "gles3")
    target_compile_definitions(sdgl_backend PUBLIC -DSDGL_GRAPHICS_GLES3)
elseif("${SDGL_GRAPHICS_LIB}" STREQUAL "sdl3")
    message(FATAL_ERROR "SDL3 shader library is not implemented yet (or released)")
endif()
