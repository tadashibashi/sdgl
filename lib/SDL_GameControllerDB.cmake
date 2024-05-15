include(FetchContent)

FetchContent_Declare(SDL_GameControllerDB
    GIT_REPOSITORY https://github.com/mdqinc/SDL_GameControllerDB.git
    GIT_TAG        9b669d72063bd57cddab0d9e411e6f669bbfb7aa
)
FetchContent_MakeAvailable(SDL_GameControllerDB)

include(copy_file)
function(copy_gamepad_mappings)
    if (ARGV0)
        set(TARGET_NAME ${ARGV0})
    else()
        set(TARGET_NAME ${PROJECT_NAME})
    endif()

    set (MAPFILE ${CMAKE_CURRENT_BINARY_DIR}/gamecontrollerdb.txt)
    #copy_file(${SDGL_ROOT_DIR}/lib/SDL_GameControllerDB/gamecontrollerdb.txt ${MAPFILE})

    # link mappings file into emscripten .data file
    if (EMSCRIPTEN)
        get_filename_component(MAPFILE_NAME ${MAPFILE} NAME)
        target_link_options(${TARGET_NAME} PRIVATE "SHELL:--preload-file ${MAPFILE}@${MAPFILE_NAME}")
    endif()
endfunction()
