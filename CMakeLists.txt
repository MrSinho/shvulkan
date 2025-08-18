cmake_minimum_required(VERSION 3.5)
add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)



set(SH_VULKAN_VERSION  "pre-release")
set(SH_VULKAN_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})





option(SH_VULKAN_LIB_DIR CACHE OFF)
option(SH_VULKAN_BIN_DIR CACHE OFF)


if (NOT SH_VULKAN_LIB_DIR)
set(SH_VULKAN_LIB_DIR ${SH_VULKAN_ROOT_DIR}/lib)
endif()

if (NOT SH_VULKAN_BIN_DIR)
set(SH_VULKAN_BIN_DIR ${SH_VULKAN_ROOT_DIR}/bin)
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
