
function(copy_gamepad_mappings)

    set (MAPFILE ${CMAKE_CURRENT_BINARY_DIR}/gamecontrollerdb.txt)
    add_custom_command(OUTPUT ${MAPFILE}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${SDGL_ROOT_DIR}/lib/SDL_GameControllerDB/gamecontrollerdb.txt ${MAPFILE})
    add_custom_target(${PROJECT_NAME}-copy-gamepad-mapping
        DEPENDS ${MAPFILE})
    add_dependencies(${PROJECT_NAME} ${PROJECT_NAME}-copy-gamepad-mapping)

endfunction()