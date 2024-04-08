project(sdgl_sandbox)
add_executable(sdgl_sandbox main.cpp)
target_link_libraries(sdgl_sandbox PRIVATE sdgl)

if (EMSCRIPTEN)
    set_target_properties(sdgl_sandbox PROPERTIES
            SUFFIX .html
    )
else()
    angle_copy_libs()
endif()

copy_assets(${CMAKE_CURRENT_SOURCE_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}/assets)
