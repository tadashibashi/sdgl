include(copy_assets)

function(add_sdgl_executable TARGET_NAME)
    set(oneValueArgs CONTENT_ROOT)
    set(multiValueArgs SOURCE CONTENT)
    cmake_parse_arguments(ARG "" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if (WIN32)
        set(EXE_TYPE WIN32)
    else()
        set(EXE_TYPE "")
    endif()

    add_executable(${TARGET_NAME} ${EXE_TYPE} ${ARG_SOURCE})

    target_link_libraries(${TARGET_NAME} PRIVATE sdgl)

    angles_copy_libs(${TARGET_NAME})
    copy_gamepad_mappings(${TARGET_NAME})

    if (ARG_CONTENT_ROOT)
        # TODO: set up content for macos bundles
        copy_assets(TARGET ${TARGET_NAME} FOLDER ${ARG_CONTENT_ROOT} FILES ${ARG_CONTENT})

    endif()

    if (EMSCRIPTEN)
        set_target_properties(${TARGET_NAME} PROPERTIES
            OUTPUT_NAME app
            SUFFIX .js
        )
    endif()

endfunction()