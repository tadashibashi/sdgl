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
# @param DESTINATION destination folder to build to
function(sdgl_pack_atlas INPUT_LIST DESTINATION)
    string(REPLACE ";" "," INPUT_LIST_ARG "${INPUT_LIST}")
    add_custom_target(crunch_atlas_for_${PROJECT_NAME}
            DEPENDS crunch
            COMMAND $<TARGET_FILE:crunch> ${DESTINATION} ${INPUT_LIST_ARG} -b -p -t -u -r
    )
    add_dependencies(${PROJECT_NAME} crunch_atlas_for_${PROJECT_NAME})
endfunction()
