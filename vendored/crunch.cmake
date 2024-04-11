project(crunch)

add_executable(crunch
    crunch/crunch/binary.cpp
    crunch/crunch/bitmap.cpp
    crunch/crunch/GuillotineBinPack.cpp
    crunch/crunch/hash.cpp
    crunch/crunch/lodepng.cpp
    crunch/crunch/main.cpp
    crunch/crunch/MaxRectsBinPack.cpp
    crunch/crunch/packer.cpp
    crunch/crunch/Rect.cpp
    crunch/crunch/str.cpp
)

target_include_directories(crunch PRIVATE crunch/crunch)

# Builds a crunch atlas to the specification required for sdgl engine
# @param FOLDER target folder that contains images; recursively searches it for image files
# @param DESTINATION destination location + root name <path/to/folder>/<atlas-name>
#                    file extensions will be appended to <atlas-name>
function(sdgl_pack_atlas FOLDER DESTINATION)

    file(GLOB_RECURSE ATLAS_DEPENDENCIES ${FOLDER}/**/*.png)
    string(REPLACE ";" " " ATLAS_DEP_STR "${ATLAS_DEPENDENCIES}")

    # Ensure target directory is available
    get_filename_component(PARENT_DIR ${DESTINATION} DIRECTORY)
    execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${PARENT_DIR})

    add_custom_command(OUTPUT ${DESTINATION}.bin
            DEPENDS crunch ${ATLAS_DEPENDENCIES}
            COMMAND $<TARGET_FILE:crunch> ${DESTINATION} ${FOLDER} -b -p -t -u -r
    )

    add_custom_target(sdgl_pack_atlas_for_${PROJECT_NAME} ALL
        DEPENDS ${DESTINATION}.bin
    )
endfunction()
