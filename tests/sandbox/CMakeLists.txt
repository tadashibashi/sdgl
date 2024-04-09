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

sdgl_make_symlink(${CMAKE_CURRENT_SOURCE_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}/assets)
sdgl_pack_atlas(${CMAKE_CURRENT_SOURCE_DIR}/assets/bmfont ${CMAKE_CURRENT_BINARY_DIR}/atlas/atlas_test)
