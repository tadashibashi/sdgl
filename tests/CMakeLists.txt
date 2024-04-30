set (CMAKE_CXX_STANDARD_REQUIRED 20)

project(sdgl_utests)

add_executable(sdgl_utests
        ServiceContainer.test.cpp
        Tween.test.cpp
        main.cpp
        BufferView.test.cpp
        BMFontData.test.cpp
        CrunchAtlasData.test.cpp
        BitFlags.test.cpp
)

include(FetchContent)

# Fetch Catch2 release 3.5.4
FetchContent_Declare(Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        b5373dadca40b7edc8570cf9470b9b1cb1934d40
)

FetchContent_MakeAvailable(Catch2)


target_link_libraries(sdgl_utests PRIVATE Catch2::Catch2 sdgl)

angles_copy_libs()


include(copy_assets)

copy_assets(FOLDER assets
    FILES
        bmfont/arial.fnt
        bmfont/font_0.png
)
