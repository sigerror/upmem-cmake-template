find_package(Python 3.6 EXACT COMPONENTS Development)
if(Python_FOUND)
    if(NOT DEFINED UPMEM_HOME)
        if("$ENV{UPMEM_HOME}" STREQUAL "")
            set(UPMEM_HOME "/usr")
        else()
            set(UPMEM_HOME $ENV{UPMEM_HOME})
        endif()
    endif()

    find_file(DPU_VERSION_FILE
        NAMES version
        HINTS ${UPMEM_HOME}
        PATH_SUFFIXES share/upmem)
    mark_as_advanced(DPU_VERSION_FILE)

    if(DPU_VERSION_FILE)
        file(READ ${DPU_VERSION_FILE} DPU_VERSION_STRING)
        if(DPU_VERSION_STRING MATCHES "UPMEM DPU SDK version (([0-9]+)\\.([0-9]+)\\.([0-9]+))")
            set(DPU_VERSION "${CMAKE_MATCH_1}")
            set(DPU_MAJOR_VERSION "${CMAKE_MATCH_2}")
            set(DPU_MINOR_VERSION "${CMAKE_MATCH_3}")
            set(DPU_PATCH_VERSION "${CMAKE_MATCH_4}")
        else()
            set(DPU_VERSION "")
            set(DPU_MAJOR_VERSION "")
            set(DPU_MINOR_VERSION "")
            set(DPU_PATCH_VERSION "")
        endif()
    endif()

    find_library(DPU_LIBRARY
        NAMES dpu
        HINTS ${UPMEM_HOME}/lib ${UPMEM_HOME}/lib64)
    mark_as_advanced(DPU_LIBRARY)
    
    find_path(DPU_INCLUDE_DIR
        NAMES dpu.h
        HINTS ${UPMEM_HOME}/include
        PATH_SUFFIXES dpu)
    mark_as_advanced(DPU_INCLUDE_DIR)

    find_file(DPU_TOOLCHAIN_FILE
        dpu.cmake
        HINTS ${UPMEM_HOME}
        PATH_SUFFIXES share/upmem/cmake)
    mark_as_advanced(DPU_TOOLCHAIN_FILE)

    if(DPU_LIBRARY AND DPU_INCLUDE_DIR)
        set(DPU_LIBRARIES ${DPU_LIBRARY} ${Python_LIBRARY})
        set(DPU_INCLUDE_DIRS ${DPU_INCLUDE_DIR})
        if(NOT TARGET UPMEM::DPU)
            add_library(UPMEM::DPU UNKNOWN IMPORTED)
            set_target_properties(UPMEM::DPU PROPERTIES IMPORTED_LOCATION ${DPU_LIBRARIES})
            target_include_directories(UPMEM::DPU INTERFACE ${DPU_INCLUDE_DIRS})
            target_link_libraries(UPMEM::DPU INTERFACE Python::Python)
        endif()
    endif()
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(DPU
    REQUIRED_VARS DPU_LIBRARY DPU_INCLUDE_DIR
    VERSION_VAR DPU_VERSION)
