include(copy_assets)

function(add_sdgl_executable TARGET_NAME)
    set(oneValueArgs CONTENT_ROOT ICON_FILE)
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

    if (APPLE)
        get_target_property(BINARY_DIR ${TARGET_NAME} BINARY_DIR)

        set(RESOURCES "")
        foreach(CUR_CONTENT ${ARG_CONTENT})
            list(APPEND RESOURCES ${BINARY_DIR}/${ARG_CONTENT_ROOT}/${CUR_CONTENT})
        endforeach()

        # setup resources
        if (ARG_ICON_FILE)
            set_source_files_properties("${ARG_ICON_FILE}" PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
        endif()

        foreach(CUR_RES ${RESOURCES})
            set_source_files_properties("${CUR_RES}" PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
        endforeach()

        set(FRAMEWORKS
            ${BINARY_DIR}/libEGL
        )

        add_custom_command(TARGET ${TARGET_NAME}
            POST_BUILD
            COMMAND
                install_name_tool -id "@rpath/libEGL.dylib"
                "${BINARY_DIR}/${TARGET_NAME}.app/Contents/Frameworks/libEGL.dylib"
            )

        set_target_properties(${TARGET_NAME} PROPERTIES
            MACOSX_BUNDLE                   TRUE
            MACOSX_BUNDLE_NAME              "${TARGET_NAME}"
            MACOSX_BUNDLE_VERSION           "${PROJECT_VERSION}"
            MACOSX_BUNDLE_ICON_FILE         "${ARG_ICON_FILE}"
            MACOSX_BUNDLE_GUI_IDENTIFIER    "org.${TARGET_NAME}.gui"
            # MACOSX_BUNDLE_INFO_PLIST       path/to/info.plist.in
            # MACHO_COMPATIBILITY_VERSION   "13.0.0"
            RESOURCE                        "${RESOURCES}"
            BUNDLE TRUE
            BUILD_RPATH                     "@executable_path/../Frameworks"
        )
    endif()

endfunction()