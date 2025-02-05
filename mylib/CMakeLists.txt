add_library(mylib src/mylib.cpp)
add_library(mylib::mylib ALIAS mylib)

set_target_properties(mylib PROPERTIES
    SOVERSION ${mylib_VERSION_MAJOR}
    VERSION ${mylib_VERSION}
    C_EXTENSIONS OFF
    CXX_EXTENSIONS OFF
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN ON)

target_compile_definitions(mylib
    PRIVATE MYLIB_NR_DPUS=${MYLIB_NR_DPUS}
            MYLIB_NR_TASKLETS=${MYLIB_NR_TASKLETS}
            MYLIB_MIN_XFER_SZ=${MYLIB_MIN_XFER_SZ})

target_compile_features(mylib PRIVATE c_std_99 cxx_std_17)

target_link_libraries(mylib
    PRIVATE OpenMP::OpenMP_CXX
            UPMEM::DPU)

target_include_directories(mylib
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
           ${CMAKE_CURRENT_BINARY_DIR}/include
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
            ${PROJECT_BINARY_DIR}/kernel/include)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/include/mylib/mylib_version.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/include/mylib/mylib_version.h)

include(GenerateExportHeader)
generate_export_header(mylib
    EXPORT_FILE_NAME ${CMAKE_CURRENT_BINARY_DIR}/include/mylib/mylib_export.h)

set_target_properties(mylib PROPERTIES
    PUBLIC_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/include/mylib/mylib.h
                  ${CMAKE_CURRENT_BINARY_DIR}/include/mylib/mylib_export.h
                  ${CMAKE_CURRENT_BINARY_DIR}/include/mylib/mylib_version.h)

include(GNUInstallDirs)
install(TARGETS mylib
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/mylib)
