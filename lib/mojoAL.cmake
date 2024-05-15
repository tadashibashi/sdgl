# Inject MojoAL into target source
# Make sure that TARGET is linked to SDL2::SDL2!
function(mojoal_inject TARGET)
    set(MOJOAL_ROOT ${SDGL_ROOT_DIR}/lib/mojoAL)
    target_sources(${TARGET} PUBLIC ${MOJOAL_ROOT}/mojoal.c)
    target_include_directories(${TARGET}
        PUBLIC ${MOJOAL_ROOT}/AL
        PUBLIC ${MOJOAL_ROOT})
endfunction()
