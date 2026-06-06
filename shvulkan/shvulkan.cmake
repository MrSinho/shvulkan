cmake_minimum_required(VERSION 3.5...4.1.2)
add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)



if (NOT DEFINED SH_VULKAN_LIB_DIR)
message(FATAL_ERROR "shvulkan cmake error: missing SH_VULKAN_LIB_DIR")
endif()

if (NOT DEFINED SH_VULKAN_ROOT_DIR)
message(FATAL_ERROR "shvulkan cmake error: missing SH_VULKAN_ROOT_DIR")
endif()



function(build_shvulkan)

find_package(Vulkan)

if (NOT DEFINED Vulkan_INCLUDE_DIR)
message(FATAL_ERROR "shvulkan cmake error: missing Vulkan_INCLUDE_DIR")
endif()

if (NOT DEFINED Vulkan_LIBRARY)
message(FATAL_ERROR "shvulkan cmake error: missing Vulkan_LIBRARY")
endif()

message(STATUS "shvulkan message: found Vulkan")
message(STATUS "shvulkan message: Vulkan_INCLUDE_DIR: ${Vulkan_INCLUDE_DIR}")
message(STATUS "shvulkan message: Vulkan_LIBRARY:     ${Vulkan_LIBRARY}")

set(SH_VULKAN_LIBRARY shvulkan)

add_library(${SH_VULKAN_LIBRARY} STATIC 
    ${SH_VULKAN_ROOT_DIR}/shvulkan/src/shvulkan/shVulkan.c
)

target_include_directories(${SH_VULKAN_LIBRARY} PUBLIC 
    ${Vulkan_INCLUDE_DIR}
    ${SH_VULKAN_ROOT_DIR}/shvulkan/include
)

set(SH_VULKAN_PKG_CONFIG_SRC ${SH_VULKAN_ROOT_DIR}/shvulkan/shvulkan.pc.in)
set(SH_VULKAN_PKG_CONFIG_DST ${SH_VULKAN_ROOT_DIR}/shvulkan/shvulkan.pc)

configure_file(
    ${SH_VULKAN_PKG_CONFIG_SRC}
    ${SH_VULKAN_PKG_CONFIG_DST}
)

target_link_libraries(${SH_VULKAN_LIBRARY} PUBLIC ${Vulkan_LIBRARY})
set_target_properties(${SH_VULKAN_LIBRARY} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${SH_VULKAN_LIB_DIR})

endfunction()