if (NOT SDGL_BACKEND)
    set(SDGL_BACKEND "glfw3")
endif()

# Link libraries
set(sdgl_backend_LIBS imgui)

if (NOT EMSCRIPTEN)
    list(APPEND sdgl_backend_LIBS angle)

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
