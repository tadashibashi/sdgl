project(sdgl_utests)

add_executable(sdgl_utests
        ServiceContainer.test.cpp
        Tween.test.cpp
        main.cpp
)

target_link_libraries(sdgl_utests PRIVATE Catch2::Catch2 sdgl)

if (NOT EMSCRIPTEN)
    angle_copy_libs()
endif()