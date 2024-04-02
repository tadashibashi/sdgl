add_library(angle INTERFACE)
if (APPLE)
    set(angle_LIBS_EXT "dylib" CACHE STRING "" FORCE)
    set(angle_LIBS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/macos/" CACHE STRING "" FORCE)
endif()

target_link_libraries(angle INTERFACE
        ${angle_LIBS_DIR}/libEGL.${angle_LIBS_EXT}
        ${angle_LIBS_DIR}/libGLESv2.${angle_LIBS_EXT}
)
target_include_directories(angle INTERFACE include)

function (angle_copy_libs)
    add_custom_target(angle_copy_libs_for_${PROJECT_NAME}
            BYPRODUCTS
            ${CMAKE_CURRENT_BINARY_DIR}/libEGL.${angle_LIBS_EXT}
            ${CMAKE_CURRENT_BINARY_DIR}/libGLESv2.${angle_LIBS_EXT}
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${angle_LIBS_DIR}/libEGL.${angle_LIBS_EXT} ${CMAKE_CURRENT_BINARY_DIR}/libEGL.${angle_LIBS_EXT}
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${angle_LIBS_DIR}/libGLESv2.${angle_LIBS_EXT} ${CMAKE_CURRENT_BINARY_DIR}/libGLESv2.${angle_LIBS_EXT}
    )
    add_dependencies(${PROJECT_NAME} angle_copy_libs_for_${PROJECT_NAME})
endfunction()