cmake_minimum_required(VERSION 3.5)
add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)



option(SH_VULKAN_VK_SDK_PATH     CACHE sdk_path)
option(SH_VULKAN_VK_INCLUDE_DIRS CACHE include_dirs)



function(build_shvulkan)

find_package(Vulkan REQUIRED)

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