
# FIXME: add function to copy assets, for now, it just copies a symlink
#        have it depend on the current target and build post or pre-build
function(copy_assets TARGET DEST)

    execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink ${TARGET} ${DEST})

endfunction()