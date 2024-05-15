include(copy_assets)

function(add_sdgl_executable TARGET_NAME)
    # Parse args
    set(flags "")
    set(oneValueArgs CONTENT_ROOT ICON_FILE)
    set(multiValueArgs SOURCE CONTENT)
    cmake_parse_arguments(ARG "${flags}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    # Parse version
    if (PROJECT_VERSION)
        set(TARGET_VERSION ${PROJECT_VERSION})
    else()
        set(TARGET_VERSION "0.0.1")
    endif()

    # Detect executable type
    if (WIN32)
        set(EXE_TYPE WIN32)
    elseif(APPLE)
        set(EXE_TYPE MACOSX_BUNDLE)
        set(CMAKE_OSX_DEPLOYMENT_TARGET 12)
    endif()

    add_executable(${TARGET_NAME} ${EXE_TYPE})
    target_link_libraries(${TARGET_NAME} PRIVATE sdgl)

    get_target_property(BINARY_DIR ${TARGET_NAME} BINARY_DIR)

    angles_copy_libs(${TARGET_NAME})
    copy_gamepad_mappings(${TARGET_NAME})

    # Copy assets to binary root
    if (ARG_CONTENT_ROOT AND NOT APPLE)
        copy_assets(TARGET ${TARGET_NAME} FOLDER ${ARG_CONTENT_ROOT} FILES ${ARG_CONTENT})
    endif()

    if (ARG_ICON_FILE AND NOT APPLE)
        copy_assets(TARGET ${TARGET_NAME} FILES ${ARG_ICON_FILE})
    endif()

    if (APPLE)
        # bundle everything together
        # icon file
        if (ARG_ICON_FILE)
            list(APPEND ARG_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/${ARG_ICON_FILE}")
            set_source_files_properties(${ARG_ICON_FILE} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
        endif()

        # gamepad mapping file
        set(GAMECONTROLLERDB_PATH "${BINARY_DIR}/gamecontrollerdb.txt")
        if (EXISTS "${GAMECONTROLLERDB_PATH}")
            list(APPEND ARG_SOURCE "${GAMECONTROLLERDB_PATH}")
            set_source_files_properties("${GAMECONTROLLERDB_PATH}" PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
        endif()

        # assets
        foreach(CUR_CONTENT ${ARG_CONTENT})
            set(CUR_CONTENT "${ARG_CONTENT_ROOT}/${CUR_CONTENT}")
            list(APPEND ARG_SOURCE ${CUR_CONTENT})
            set_source_files_properties("${CUR_CONTENT}" PROPERTIES MACOSX_PACKAGE_LOCATION "Resources/${ARG_CONTENT_ROOT}")
        endforeach()

        # dynamic libraries (copied over to binary dir from copy_assets)
        file(GLOB TARGET_LIBRARIES ${BINARY_DIR}/*.dylib)
        list(APPEND ARG_SOURCE ${TARGET_LIBRARIES})
        set_source_files_properties(${TARGET_LIBRARIES} PROPERTIES MACOSX_PACKAGE_LOCATION "Frameworks")

        if (CMAKE_BUILD_TYPE MATCHES "Rel")
            set(CODE_SIGN_ID "Apple Development")
        endif()

        set_target_properties(${TARGET_NAME} PROPERTIES
            MACOSX_BUNDLE                   YES
            MACOSX_BUNDLE_BUNDLE_NAME       "${TARGET_NAME}"
            MACOSX_BUNDLE_BUNDLE_VERSION    "${TARGET_VERSION}"
            MACOSX_BUNDLE_ICON_FILE         "${ARG_ICON_FILE}"
            MACOSX_BUNDLE_GUI_IDENTIFIER    "org.${TARGET_NAME}.gui"
            MACOSX_BUNDLE_INFO_PLIST        "" # can customize later...

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
            OUTPUT_NAME app # TODO: allow project name for file, but use
            SUFFIX .js
        )

        # Copy .html shell and .js driver to the output directory
        # TODO: make html shell configurable through cmake configure_file
        copy_file(${SDGL_ROOT_DIR}/src/sdgl/platform/index.html ${BINARY_DIR}/index.html)
        copy_file(${SDGL_ROOT_DIR}/src/sdgl/platform/main.js ${BINARY_DIR}/main.js)

    endif()

    target_sources(${TARGET_NAME} PRIVATE ${ARG_SOURCE})
endfunction()
