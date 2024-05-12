function(copy_file SOURCE DEST)
    execute_process(COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SOURCE} ${DEST})
endfunction()
