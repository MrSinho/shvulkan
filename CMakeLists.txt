cmake_minimum_required(VERSION 3.0)
add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)

project(shvulkan C)

option(SH_VULKAN_VK_SDK_PATH CACHE sdk_path)
option(SH_VULKAN_VK_INCLUDE_DIRS CACHE include_dirs)

find_package(Vulkan REQUIRED)
message(STATUS "shvulkan message: found Vulkan")
message(STATUS "shvulkan message: Vulkan_INCLUDE_DIR: ${Vulkan_INCLUDE_DIR}")
message(STATUS "shvulkan message: Vulkan_LIBRARY:     ${Vulkan_LIBRARY}")


function(build_shvulkan)
set(SH_VULKAN_SRC_DIR shvulkan/src/shvulkan)
add_library(shvulkan STATIC 
    ${SH_VULKAN_SRC_DIR}/shVulkan.c
)
target_include_directories(shvulkan PUBLIC 
    ${Vulkan_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/shvulkan/include
)
target_link_libraries(shvulkan PUBLIC ${Vulkan_LIBRARY})
set_target_properties(shvulkan PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
endfunction()



function(build_shvulkan_examples)
add_subdirectory(examples/external/glfw)

add_executable(shvulkan-compute-power-numbers examples/src/compute/power-numbers.c)
add_executable(shvulkan-clear-color           examples/src/graphics/clear-color.c)
add_executable(shvulkan-resizable-clear-color examples/src/graphics/resizable-clear-color.c)
add_executable(shvulkan-scene                 examples/src/graphics/scene.c)
#add_executable(shvulkan-texture               examples/src/graphics/texture.c)

target_link_libraries(shvulkan-compute-power-numbers PUBLIC shvulkan)
if (WIN32)
target_link_libraries(shvulkan-clear-color           PUBLIC shvulkan glfw)
target_link_libraries(shvulkan-resizable-clear-color PUBLIC shvulkan glfw)
target_link_libraries(shvulkan-scene                 PUBLIC shvulkan glfw)
target_link_libraries(shvulkan-scene                 PUBLIC shvulkan glfw)
#target_link_libraries(shvulkan-texture               PUBLIC shvulkan glfw)
elseif (UNIX)
target_link_libraries(shvulkan-clear-color           PUBLIC shvulkan glfw X11)
target_link_libraries(shvulkan-resizable-clear-color PUBLIC shvulkan glfw X11)
target_link_libraries(shvulkan-scene                 PUBLIC shvulkan glfw X11)
#target_link_libraries(shvulkan-texture               PUBLIC shvulkan glfw X11)
endif(WIN32)

set_target_properties(
    shvulkan-compute-power-numbers 
    shvulkan-clear-color 
    shvulkan-resizable-clear-color 
    shvulkan-scene
    #shvulkan-texture

    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
    VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
)
endfunction()



option(SH_VULKAN_BUILD_EXAMPLES CACHE OFF)



function(main)
build_shvulkan()
if (SH_VULKAN_BUILD_EXAMPLES)
build_shvulkan_examples()
endif(SH_VULKAN_BUILD_EXAMPLES)
endfunction()



main()
