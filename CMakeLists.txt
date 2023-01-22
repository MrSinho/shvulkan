cmake_minimum_required(VERSION 3.0)
add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)

project(shvulkan C)

find_package(Vulkan REQUIRED)



function(build_shvulkan)
set(SH_VULKAN_SRC_DIR shvulkan/src/shvulkan)
add_library(shvulkan STATIC 
    ${SH_VULKAN_SRC_DIR}/shVulkan.c
)
target_include_directories(shvulkan PUBLIC 
    ${Vulkan_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/shvulkan/include
)
target_link_libraries(shvulkan PUBLIC ${Vulkan_LIBRARIES})
set_target_properties(shvulkan PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
endfunction()



function(build_shvulkan_examples)
add_subdirectory(examples/external/glfw)

add_executable(shvulkan-clear-color examples/src/graphics/clear-color.c)

add_executable(shvulkan-resizable-clear-color examples/src/graphics/resizable-clear-color.c)

add_executable(shvulkan-scene examples/src/graphics/triangle.c)

if (WIN32)
target_link_libraries(
    shvulkan-clear-color 
    PUBLIC 
    shvulkan glfw
)
target_link_libraries(
    shvulkan-resizable-clear-color 
    PUBLIC 
    shvulkan glfw
)
target_link_libraries(
    shvulkan-scene
    PUBLIC 
    shvulkan glfw
)
elseif (UNIX)
target_link_libraries(
    shvulkan-clear-color 
    PUBLIC 
    shvulkan glfw X11
)
target_link_libraries(
    shvulkan-resizable-clear-color 
    PUBLIC 
    shvulkan glfw X11
)
target_link_libraries(
    shvulkan-scene
    PUBLIC 
    shvulkan glfw X11
)
endif(WIN32)

add_executable(shvulkan-compute-power-numbers examples/src/compute/power-numbers.c)
target_link_libraries(shvulkan-compute-power-numbers PUBLIC shvulkan)

set_target_properties(
    shvulkan-clear-color 
    shvulkan-resizable-clear-color 
    shvulkan-compute-power-numbers 

    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
    VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
)
endfunction()



option(SH_USE_VULKAN_1_0 CACHE OFF)
if(SH_USE_VULKAN_1_0)
    add_definitions(-DSH_USE_VULKAN_1_0)
endif(SH_USE_VULKAN_1_0)

option(SH_USE_VULKAN_1_1 CACHE OFF)
if(SH_USE_VULKAN_1_1)
    add_definitions(-DSH_USE_VULKAN_1_1)
endif(SH_USE_VULKAN_1_1)

option(SH_VULKAN_BUILD_EXAMPLES CACHE OFF)



function(main)
build_shvulkan()
if (SH_VULKAN_BUILD_EXAMPLES)
build_shvulkan_examples()
endif(SH_VULKAN_BUILD_EXAMPLES)
endfunction()



main()
