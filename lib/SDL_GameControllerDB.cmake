
function(copy_gamepad_mappings)
    if (${ARGV0})
        set(TARGET_NAME ${ARGV0})
    else()
        set(TARGET_NAME ${PROJECT_NAME})
    endif()

    set (MAPFILE ${CMAKE_CURRENT_BINARY_DIR}/gamecontrollerdb.txt)
    add_custom_command(OUTPUT ${MAPFILE}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${SDGL_ROOT_DIR}/lib/SDL_GameControllerDB/gamecontrollerdb.txt ${MAPFILE})
    add_custom_target(${TARGET_NAME}-copy-gamepad-mapping
        DEPENDS ${MAPFILE})
    add_dependencies(${TARGET_NAME} ${TARGET_NAME}-copy-gamepad-mapping)

    # link mappings file into emscripten .data file
    if (EMSCRIPTEN)
        target_link_options(${TARGET_NAME} PRIVATE --preload-file ${MAP_FILE})
    endif()

endfunction()
