
option(IMGUI_INCLUDE_DEMO "Include ImGui demo window source" ON)

set(imgui_SRC
    imgui/imgui.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
)

if (IMGUI_INCLUDE_DEMO)
    list(APPEND imgui_SRC imgui/imgui_demo.cpp)
endif()

add_library(imgui STATIC ${imgui_SRC})
target_include_directories(imgui PUBLIC imgui imgui/backends)
