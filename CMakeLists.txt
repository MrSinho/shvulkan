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
set(SH_VULKAN_SRC_DIR shvulkan/src/shvulkan)
add_library(shvulkan STATIC 
    ${SH_VULKAN_SRC_DIR}/shVkCore.c
    ${SH_VULKAN_SRC_DIR}/shVkMemoryInfo.c
    ${SH_VULKAN_SRC_DIR}/shVkPipelineData.c
    ${SH_VULKAN_SRC_DIR}/shVkCheck.c
    ${SH_VULKAN_SRC_DIR}/shVkDrawLoop.c
)
target_include_directories(shvulkan PUBLIC 
    ${Vulkan_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/shvulkan/include
)
target_link_libraries(shvulkan PUBLIC ${Vulkan_LIBRARIES})
set_target_properties(shvulkan PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

option(SH_VULKAN_BUILD_EXAMPLES OFF)
if (SH_VULKAN_BUILD_EXAMPLES)
add_subdirectory(examples/external/glfw)
add_executable(shvulkan-graphics-example examples/src/graphicsExample.c)
target_link_libraries(shvulkan-graphics-example PUBLIC shvulkan glfw)
add_executable(shvulkan-compute-example examples/src/computeExample.c)
target_link_libraries(shvulkan-compute-example PUBLIC shvulkan glfw)
set_target_properties(shvulkan-graphics-example shvulkan-compute-example PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
    VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
)
endif(SH_VULKAN_BUILD_EXAMPLES)