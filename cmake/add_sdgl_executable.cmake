include(copy_assets)

function(add_sdgl_executable TARGET_NAME)
    set(oneValueArgs CONTENT_ROOT ICON_FILE)
    set(multiValueArgs SOURCE CONTENT)
    cmake_parse_arguments(ARG "" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if (WIN32)
        set(EXE_TYPE WIN32)
    elseif(APPLE)
        set(EXE_TYPE MACOSX_BUNDLE)
    endif()

    add_executable(${TARGET_NAME} ${EXE_TYPE})
    get_target_property(TARGET_VERSION ${TARGET_NAME} VERSION)
    if (NOT TARGET_VERSION)
        set(TARGET_VERSION 0.0.1)
    endif()

    target_link_libraries(${TARGET_NAME} PRIVATE sdgl)

    angles_copy_libs(${TARGET_NAME})
    copy_gamepad_mappings(${TARGET_NAME})

    # copy assets to binary root
    if (ARG_CONTENT_ROOT)
        copy_assets(TARGET ${TARGET_NAME} FOLDER ${ARG_CONTENT_ROOT} FILES ${ARG_CONTENT} ICON_FILE ${ARG_ICON_FILE})
    endif()

    if (APPLE)
        get_target_property(BINARY_DIR ${TARGET_NAME} BINARY_DIR)

        # bundle every thing together
        # icon file
        if (ARG_ICON_FILE)
            list(APPEND ARG_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/${ARG_ICON_FILE}")
            set_source_files_properties(${ARG_ICON_FILE} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
        endif()

        # assets
        foreach(CUR_CONTENT ${ARG_CONTENT})
            set(CUR_RESOURCE "${BINARY_DIR}/${ARG_CONTENT_ROOT}/${CUR_CONTENT}")
            list(APPEND ARG_SOURCE ${CUR_RESOURCE})
            set_source_files_properties("${CUR_RESOURCE}" PROPERTIES MACOSX_PACKAGE_LOCATION "Resources/${ARG_CONTENT_ROOT}")
        endforeach()

        # dynamic libraries (copied over to binary dir from copy_assets)
        file(GLOB TARGET_LIBRARIES ${BINARY_DIR}/*.dylib)
        list(APPEND ARG_SOURCE ${TARGET_LIBRARIES})
        set_source_files_properties(${TARGET_LIBRARIES} PROPERTIES MACOSX_PACKAGE_LOCATION "Frameworks")

        if (CMAKE_BUILD_TYPE MATCHES "Release" OR CMAKE_BUILD_TYPE MATCHES "RelWithDebInfo" OR CMAKE_BUILD_TYPE MATCHES "MinSizeRel")
            set(CODE_SIGN_ID "Apple Development")
        endif()

        set_target_properties(${TARGET_NAME} PROPERTIES
            MACOSX_BUNDLE                   YES
            MACOSX_BUNDLE_BUNDLE_NAME       "${TARGET_NAME}"
            MACOSX_BUNDLE_BUNDLE_VERSION    "${TARGET_VERSION}"
            MACOSX_BUNDLE_ICON_FILE         "${ARG_ICON_FILE}"
            MACOSX_BUNDLE_GUI_IDENTIFIER    "org.${TARGET_NAME}.gui"
            MACOSX_BUNDLE_INFO_PLIST        "" # can customize later...
            MACHO_COMPATIBILITY_VERSION     "13.0.0" # lowest version to support

            BUILD_RPATH                     "@executable_path/../Frameworks"

            XCODE_EMBED_FRAMEWORKS_CODE_SIGN_ON_COPY    YES
            XCODE_EMBED_RESOURCES_CODE_SIGN_ON_COPY     YES
            XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY          "${CODE_SIGN_ID}"
            XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER   "org.${TARGET_NAME}.gui"
            XCODE_ATTRIBUTE_GENERATE_INFOPLIST_FILE     YES
        )

        install(TARGETS ${TARGET_NAME} BUNDLE DESTINATION ${CMAKE_INSTALL_PREFIX})

    elseif(EMSCRIPTEN)

        set_target_properties(${TARGET_NAME} PROPERTIES
            OUTPUT_NAME app
            SUFFIX .js
        )
    endif()

    target_sources(${TARGET_NAME} PRIVATE ${ARG_SOURCE})
endfunction()
