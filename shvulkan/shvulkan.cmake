cmake_minimum_required(VERSION 3.5)
add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)



if (NOT DEFINED SH_VULKAN_BINARIES_DIR)
message(FATAL_ERROR "shvulkan cmake error: missing SH_VULKAN_BINARIES_DIR")
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

add_library(shvulkan STATIC 
    ${SH_VULKAN_ROOT_DIR}/shvulkan/src/shvulkan/shVulkan.c
)

target_include_directories(shvulkan PUBLIC 
    ${Vulkan_INCLUDE_DIR}
    ${SH_VULKAN_ROOT_DIR}/shvulkan/include
)

target_link_libraries(shvulkan PUBLIC ${Vulkan_LIBRARY})
set_target_properties(shvulkan PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${SH_VULKAN_BINARIES_DIR})

endfunction()