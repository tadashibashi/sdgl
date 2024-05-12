include(copy_file)

function(copy_asset TARGET SOURCE DEST)
    add_custom_command(TARGET ${TARGET} PRE_LINK
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${SOURCE} ${DEST})
endfunction()

# Copy a folder of assets to a target's binary directory
function(copy_assets)
    set(oneValueArgs FOLDER TARGET) # only need to set TARGET if it differs from project name,
                                    # target exe should be in same folder as this function call
    set(multiValueArgs FILES)
    cmake_parse_arguments(ARG "" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    if (ARG_TARGET)
        set(TARGET ${ARG_TARGET})
    else()
        set(TARGET ${PROJECT_NAME})
    endif()

    # Setup variables
    get_target_property(BINARY_DIR ${TARGET} BINARY_DIR)
    set(LOCAL_ASSET_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${ARG_FOLDER})
    set(TARGET_ASSET_DIR ${BINARY_DIR}/${ARG_FOLDER})

    # Emscripten flags
    if (EMSCRIPTEN)
        if (ARG_FOLDER)
            target_link_options(${TARGET} PRIVATE "SHELL:--preload-file ${TARGET_ASSET_DIR}@${ARG_FOLDER}")
        else()
            foreach(FILE ${ARG_FILES})
                target_link_options(${TARGET} PRIVATE "SHELL:--preload-file ${TARGET_ASSET_DIR}/${FILE}@${FILE}")
            endforeach()
        endif()
    endif()

    # Copy files to into binary asset dir
    foreach(FILE ${ARG_FILES})
        # Copy Assets
        copy_file(${LOCAL_ASSET_DIR}/${FILE} ${TARGET_ASSET_DIR}/${FILE})
    endforeach()

endfunction()
