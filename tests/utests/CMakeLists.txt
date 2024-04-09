set (CMAKE_CXX_STANDARD_REQUIRED 20)

project(sdgl_utests)

add_executable(sdgl_utests
        ServiceContainer.test.cpp
        Tween.test.cpp
        main.cpp
        PriorityList.test.cpp
        BufferView.test.cpp
        BMFontData.test.cpp
        CrunchAtlasData.test.cpp
)

target_link_libraries(sdgl_utests PRIVATE Catch2::Catch2 sdgl)

if (NOT EMSCRIPTEN)
    angle_copy_libs()
endif()

sdgl_make_symlink(${CMAKE_CURRENT_SOURCE_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}/assets)
sdgl_pack_atlas(${CMAKE_CURRENT_SOURCE_DIR}/assets/atlas ${CMAKE_CURRENT_BINARY_DIR}/atlas)