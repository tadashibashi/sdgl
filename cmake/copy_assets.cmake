# FIXME: add function to copy assets, for now, it just copies a symlink
#        have it depend on the current target and build post or pre-build
function(copy_assets)
    set(oneValueArgs FOLDER TARGET) # only need to set TARGET if it differs from project name,
                                    # target exe should be in same folder as this function call
    set(multiValueArgs FILES)
    cmake_parse_arguments(ARG "" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    if (NOT ARG_TARGET)
        set(ARG_TARGET ${PROJECT_NAME})
    endif()
    get_target_property(BINARY_DIR ${ARG_TARGET} BINARY_DIR)

    set(LOCAL_ASSET_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${ARG_FOLDER})

    # set target asset directory
    if (APPLE) # app Resources
        set(TARGET_ASSET_DIR ${BINARY_DIR}/${ARG_TARGET}.app/Contents/Resources/${ARG_FOLDER})
    else()    # binary dir
        set(TARGET_ASSET_DIR ${BINARY_DIR}/${ARG_FOLDER})
    endif()

    # Set emscripten flags
    if (EMSCRIPTEN)
        target_link_options(${ARG_TARGET} PRIVATE --preload-file ${TARGET_ASSET_DIR}@${ARG_FOLDER})

        # copy .html shell and .js to the output directory
        if (NOT EXISTS ${BINARY_DIR}/index.html)
            execute_process(COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${SDGL_ROOT_DIR}/src/sdgl/platform/index.html ${BINARY_DIR}/index.html)
        endif()

        if (NOT EXISTS ${BINARY_DIR}/main.js)
            execute_process(COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${SDGL_ROOT_DIR}/src/sdgl/platform/main.js ${BINARY_DIR}/main.js)
        endif()
    endif()

    fmod_copy_libs()

    # Copy files to into binary asset dir
    foreach(FILE ${ARG_FILES})
        # Copy Assets
        add_custom_command(
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${LOCAL_ASSET_DIR}/${FILE} ${TARGET_ASSET_DIR}/${FILE}
            OUTPUT ${TARGET_ASSET_DIR}/${FILE}
        )

        list(APPEND DEPS ${TARGET_ASSET_DIR}/${FILE})
    endforeach()

    add_custom_target(${ARG_TARGET}-copy-assets
        DEPENDS "${DEPS}"
    )

    add_dependencies(${ARG_TARGET} ${ARG_TARGET}-copy-assets)

    if (APPLE)
        # copy libs to bundle
        if (EXISTS ${BINARY_DIR}/libEGL.dylib AND NOT EXISTS ${BINARY_DIR}/${ARG_TARGET}.app/Contents/Frameworks/libEGL.dylib)
            execute_process(COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${BINARY_DIR}/libEGL.dylib ${BINARY_DIR}/${ARG_TARGET}.app/Contents/Frameworks/libEGL.dylib
            )
        endif()

        if (EXISTS ${BINARY_DIR}/libGLESv2.dylib AND NOT EXISTS ${BINARY_DIR}/${ARG_TARGET}.app/Contents/Frameworks/libGLESv2.dylib)
            execute_process(COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${BINARY_DIR}/libGLESv2.dylib ${BINARY_DIR}/${ARG_TARGET}.app/Contents/Frameworks/libGLESv2.dylib
            )
        endif()

        foreach(LIB ${FMOD_LIBS})
            get_filename_component(FMOD_LIB_NAME ${LIB} NAME)
            execute_process(COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${BINARY_DIR}/${FMOD_LIB_NAME} ${BINARY_DIR}/${ARG_TARGET}.app/Contents/Frameworks/${FMOD_LIBNAME}
                )
        endforeach()

        foreach(LIB ${FMOD_STUDIO_LIBS})
            get_filename_component(FMOD_LIB_NAME ${LIB} NAME)
            execute_process(COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${BINARY_DIR}/${FMOD_LIB_NAME} ${BINARY_DIR}/${ARG_TARGET}.app/Contents/Frameworks/${FMOD_LIBNAME}
                )
        endforeach()
    endif()

endfunction()