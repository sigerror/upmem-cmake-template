cmake_minimum_required(VERSION 3.16)
project(mylib_kernel LANGUAGES C)

set(KERNEL_SOURCES src/sum.c)

foreach(K_SOURCE IN LISTS KERNEL_SOURCES)
    get_filename_component(K_NAME ${K_SOURCE} NAME_WE)
    add_executable(${K_NAME} ${K_SOURCE})
    target_compile_definitions(${K_NAME} PRIVATE NR_TASKLETS=${NR_TASKLETS})
    target_link_options(${K_NAME} PRIVATE -DNR_TASKLETS=${NR_TASKLETS})
    
    string(TOUPPER "MYLIB_KERNEL_${K_NAME}_H" K_INCLUDE_GUARD)
    configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/include/kernel/kernel.h.in
        ${CMAKE_CURRENT_BINARY_DIR}/include/kernel/${K_NAME}.h)
    
    set_target_properties(${K_NAME} PROPERTIES
        PUBLIC_HEADER ${CMAKE_CURRENT_BINARY_DIR}/include/kernel/${K_NAME}.h)
    install(TARGETS ${K_NAME} PUBLIC_HEADER DESTINATION include/kernel)
endforeach()
