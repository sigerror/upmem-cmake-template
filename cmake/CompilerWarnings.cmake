function(mylib_set_compiler_warnings target)
    target_compile_options(${target} INTERFACE
        $<$<OR:$<C_COMPILER_ID:Clang,GNU>,$<CXX_COMPILER_ID:Clang,GNU>>:
            -Wall;
            -Wextra;
            -Wshadow;
            -Wnon-virtual-dtor;
            -Wold-style-cast;
            -Wcast-align;
            -Wunused;
            -Woverloaded-virtual;
            -Wpedantic;
            -Wconversion;
            -Wsign-conversion;
            -Wnull-dereference;
            -Wdouble-promotion;
            -Wformat=2;
            -Wimplicit-fallthrough>
        $<$<OR:$<C_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:GNU>>:
            -Wmisleading-indentation;
            -Wduplicated-cond;
            -Wduplicated-branches;
            -Wlogical-op;
            -Wuseless-cast;
            -Wsuggest-override>)
endfunction()
