function(ngine_set_compiler_warnings target)
    target_compile_options(${target} PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:
            /W4 /WX /permissive-
            /w14242 /w14254 /w14263 /w14265 /w14287
            /w14296 /w14311 /w14545 /w14546 /w14547
            /w14549 /w14555 /w14619 /w14640 /w14826
            /w14905 /w14906 /w14928
        >
        $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:
            -Wall -Wextra -Wpedantic -Werror
            -Wshadow -Wnon-virtual-dtor -Wold-style-cast
            -Wcast-align -Wunused -Woverloaded-virtual
            -Wconversion -Wsign-conversion -Wnull-dereference
            -Wdouble-promotion -Wformat=2
            -Wimplicit-fallthrough
        >
    )
endfunction()
