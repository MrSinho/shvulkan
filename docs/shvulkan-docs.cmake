cmake_minimum_required(VERSION 3.5)
add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)



function(build_shvulkan_docs)



if (NOT DEFINED SH_VULKAN_DOCS_DIR)
message(FATAL_ERROR "shvulkan cmake error: missing SH_VULKAN_DOCS_DIR")
endif()

if (NOT DEFINED SH_VULKAN_ROOT_DIR)
message(FATAL_ERROR "shvulkan cmake error: missing SH_VULKAN_ROOT_DIR")
endif()




find_package(Doxygen)


if (DOXYGEN_FOUND)

    set(SH_VULKAN_DOXYFILESRC ${SH_VULKAN_DOCS_DIR}/Doxyfile.in)
    set(SH_VULKAN_DOXYFILEDST ${SH_VULKAN_DOCS_DIR}/Doxyfile)

    message(STATUS "Configuring " ${SH_VULKAN_DOXYFILEDST} " from " ${SH_VULKAN_DOXYFILESRC})

    configure_file(
        ${SH_VULKAN_DOXYFILESRC} 
        ${SH_VULKAN_DOXYFILEDST}
    )

    message(STATUS "Creating custom docs command: " ${DOXYGEN_EXECUTABLE} " " ${SH_VULKAN_DOXYFILEDST})

    add_custom_target(docs-shvulkan
        COMMAND ${DOXYGEN_EXECUTABLE} ${SH_VULKAN_DOXYFILEDST}
        WORKING_DIRECTORY ${SH_VULKAN_ROOT_DIR}
        COMMENT "Generating HTML documentation"
        VERBATIM
    )

endif(DOXYGEN_FOUND)

endfunction()