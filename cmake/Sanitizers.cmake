function(ngine_enable_sanitizers target)
    if(NOT NGINE_ENABLE_SANITIZERS)
        return()
    endif()

    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        target_compile_options(${target} PRIVATE
            -fsanitize=address,undefined
            -fno-sanitize-recover=all
            -fno-omit-frame-pointer
        )
        target_link_options(${target} PRIVATE
            -fsanitize=address,undefined
        )
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        target_compile_options(${target} PRIVATE /fsanitize=address)
    endif()
endfunction()
