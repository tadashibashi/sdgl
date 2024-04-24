project(sdgl_content_pipeline CXX)

set(CMAKE_CXX_STANDARD_REQUIRED 20)

add_executable(sdgl_content_pipeline
        main.cpp
        FileWatcherCache.h
        FileWatcherCache.cpp
        ContentConfig.h
        cute_aseprite.cpp
)

target_link_libraries(sdgl_content_pipeline PRIVATE sdgl)
angles_copy_libs()
