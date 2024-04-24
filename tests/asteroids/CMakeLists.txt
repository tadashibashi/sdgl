project(asteroids)
add_executable(asteroids main.cpp
        Asteroids.h
        Sprite.h)
target_link_libraries(asteroids PRIVATE sdgl)

if (EMSCRIPTEN)
    set_target_properties(asteroids PROPERTIES
            SUFFIX .html
    )
else()
    angles_copy_libs()
endif()

sdgl_pack_atlas(${CMAKE_CURRENT_SOURCE_DIR}/assets/images ${CMAKE_CURRENT_BINARY_DIR}/atlas/sprites)
