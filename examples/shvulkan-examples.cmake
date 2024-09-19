cmake_minimum_required(VERSION 3.5)
add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)



if (NOT DEFINED SH_VULKAN_BINARIES_DIR)
message(FATAL_ERROR "shvulkan cmake error: missing SH_VULKAN_BINARIES_DIR")
endif()

if (NOT DEFINED SH_VULKAN_ROOT_DIR)
message(FATAL_ERROR "shvulkan cmake error: missing SH_VULKAN_ROOT_DIR")
endif()



function(build_shvulkan_examples)

add_subdirectory(examples/external/glfw)

add_executable(shvulkan-compute-power-numbers examples/src/compute/power-numbers.c)
add_executable(shvulkan-clear-color           examples/src/graphics/clear-color.c)
add_executable(shvulkan-scene                 examples/src/graphics/scene.c)

target_link_libraries(shvulkan-compute-power-numbers PUBLIC shvulkan)

if (WIN32)
target_link_libraries(shvulkan-clear-color           PUBLIC shvulkan glfw)
target_link_libraries(shvulkan-scene                 PUBLIC shvulkan glfw)
target_link_libraries(shvulkan-scene                 PUBLIC shvulkan glfw)
elseif (UNIX)
target_link_libraries(shvulkan-clear-color           PUBLIC shvulkan glfw X11)
target_link_libraries(shvulkan-scene                 PUBLIC shvulkan glfw X11)
endif(WIN32)

set_target_properties(
    shvulkan-compute-power-numbers 
    shvulkan-clear-color 
    shvulkan-scene

    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY      ${SH_VULKAN_BINARIES_DIR}
    VS_DEBUGGER_WORKING_DIRECTORY ${SH_VULKAN_BINARIES_DIR}
)

endfunction()