project(sdgl_tests)

add_executable(sdgl_tests
    sdgl/ServiceContainer.test.cpp
)
target_link_libraries(sdgl_tests PRIVATE Catch2::Catch2 sdgl)
