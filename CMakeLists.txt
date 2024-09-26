cmake_minimum_required(VERSION 3.5)
add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)



set(SH_VULKAN_VERSION  "pre-release")
set(SH_VULKAN_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})





option(SH_VULKAN_BINARIES_DIR CACHE OFF)


if (NOT SH_VULKAN_BINARIES_DIR)

if (WIN32)
set(SH_VULKAN_BINARIES_DIR ${SH_VULKAN_ROOT_DIR}/bin/windows)
elseif(UNIX)
set(SH_VULKAN_BINARIES_DIR ${SH_VULKAN_ROOT_DIR}/bin/linux)
endif(WIN32)

endif()






project(shvulkan C)






include(${SH_VULKAN_ROOT_DIR}/shvulkan/shvulkan.cmake)
include(${SH_VULKAN_ROOT_DIR}/examples/shvulkan-examples.cmake)
include(${SH_VULKAN_ROOT_DIR}/docs/shvulkan-docs.cmake)



option(SH_VULKAN_BUILD_EXAMPLES CACHE OFF)



function(main)

build_shvulkan()
build_shvulkan_docs()
if (SH_VULKAN_BUILD_EXAMPLES)
build_shvulkan_examples()
endif(SH_VULKAN_BUILD_EXAMPLES)

endfunction()



main()
