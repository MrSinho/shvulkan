cmake_minimum_required(VERSION 3.0)
add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)

project(ShVulkan C)

find_package(Vulkan REQUIRED FATAL_ERROR)

option(SH_USE_VULKAN_1_0 OFF)
if(SH_USE_VULKAN_1_0)
    add_definitions(-DSH_USE_VULKAN_1_0)
endif(SH_USE_VULKAN_1_0)

option(SH_USE_VULKAN_1_1 OFF)
if(SH_USE_VULKAN_1_1)
    add_definitions(-DSH_USE_VULKAN_1_1)
endif(SH_USE_VULKAN_1_1)

#ShVulkan
set(SH_VULKAN_SRC_DIR ShVulkan/src/shvulkan)
add_library(ShVulkan STATIC 
    ${SH_VULKAN_SRC_DIR}/shVkCore.c
    ${SH_VULKAN_SRC_DIR}/shVkMemoryInfo.c
    ${SH_VULKAN_SRC_DIR}/shVkPipelineData.c
    ${SH_VULKAN_SRC_DIR}/shVkCheck.c
    ${SH_VULKAN_SRC_DIR}/shVkDrawLoop.c
)
target_include_directories(ShVulkan PUBLIC 
    ${Vulkan_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/ShVulkan/include
)
target_link_libraries(ShVulkan PUBLIC ${Vulkan_LIBRARIES})

#ShVulkanExample
add_executable(ShVulkanExample ShVulkanExample/src/ShVulkanExample.c)
target_link_libraries(ShVulkanExample PUBLIC ShVulkan)

#OUTPUT
set_target_properties(ShVulkan ShVulkanExample PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
    VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
)