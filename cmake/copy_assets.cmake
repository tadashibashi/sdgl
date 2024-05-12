# Copy a folder of assets to a target's binary directory
function(copy_assets)
    set(oneValueArgs FOLDER TARGET ICON_FILE) # only need to set TARGET if it differs from project name,
                                    # target exe should be in same folder as this function call
    set(multiValueArgs FILES)
    cmake_parse_arguments(ARG "" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    if (NOT ARG_TARGET)
        set(ARG_TARGET ${PROJECT_NAME})
    endif()

    # Setup variables
    get_target_property(BINARY_DIR ${ARG_TARGET} BINARY_DIR)
    set(LOCAL_ASSET_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${ARG_FOLDER})
    set(TARGET_ASSET_DIR ${BINARY_DIR}/${ARG_FOLDER})

    set(ICON_FILE_NAME "")
    if (ARG_ICON_FILE)
        get_filename_component(ICON_FILE_NAME ${ARG_ICON_FILE} NAME)
    endif()

    # Emscripten flags
    if (EMSCRIPTEN)
        target_link_options(${ARG_TARGET} PRIVATE --preload-file ${TARGET_ASSET_DIR}@${ARG_FOLDER})

        # Copy .html shell and .js to the output directory
        # TODO: make html shell configurable through cmake configure_file
        # TODO: may want to move these calls to a platform setup function
        if (NOT EXISTS ${BINARY_DIR}/index.html)
            execute_process(COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${SDGL_ROOT_DIR}/src/sdgl/platform/index.html ${BINARY_DIR}/index.html)
        endif()

        if (NOT EXISTS ${BINARY_DIR}/main.js)
            execute_process(COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${SDGL_ROOT_DIR}/src/sdgl/platform/main.js ${BINARY_DIR}/main.js)
        endif()
    endif()

    # TODO: may need to add conditional here if FMOD becomes optional/decoupled
    fmod_copy_libs()

    # Copy icon file
    if (ARG_ICON_FILE)
        execute_process(COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_CURRENT_SOURCE_DIR}/${ARG_ICON_FILE} ${BINARY_DIR}/${ICON_FILE_NAME}
        )
    endif()

    # Copy files to into binary asset dir
    foreach(FILE ${ARG_FILES})
        # Copy Assets
        add_custom_command(
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${LOCAL_ASSET_DIR}/${FILE} ${TARGET_ASSET_DIR}/${FILE}
            OUTPUT ${TARGET_ASSET_DIR}/${FILE}
        )

        list(APPEND DEPS ${TARGET_ASSET_DIR}/${FILE})
    endforeach()

    # Set up copy-assets target / dependency
    add_custom_target(${ARG_TARGET}-copy-assets DEPENDS "${DEPS}")
    add_dependencies(${ARG_TARGET} ${ARG_TARGET}-copy-assets)

endfunction()
