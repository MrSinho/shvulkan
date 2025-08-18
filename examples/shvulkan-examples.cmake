cmake_minimum_required(VERSION 3.5)
add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)



if (NOT DEFINED SH_VULKAN_BIN_DIR)
message(FATAL_ERROR "shvulkan cmake error: missing SH_VULKAN_BIN_DIR")
endif()

if (NOT DEFINED SH_VULKAN_ROOT_DIR)
message(FATAL_ERROR "shvulkan cmake error: missing SH_VULKAN_ROOT_DIR")
endif()



function(build_shvulkan_examples)

add_subdirectory(${SH_VULKAN_ROOT_DIR}/examples/external/glfw)

if (NOT TARGET vvo)
set(VVO_BINARIES_DIR ${SH_VULKAN_BIN_DIR})
set(VVO_ROOT_DIR     ${SH_VULKAN_ROOT_DIR}/examples/external/vulkan-virtual-outputs)
include(${SH_VULKAN_ROOT_DIR}/examples/external/vulkan-virtual-outputs/vvo/vvo.cmake)
build_vvo()
endif()


add_executable(shvulkan-compute-power-numbers ${SH_VULKAN_ROOT_DIR}/examples/src/compute/power-numbers.c)
add_executable(shvulkan-clear-color           ${SH_VULKAN_ROOT_DIR}/examples/src/graphics/clear-color.c)
add_executable(shvulkan-scene                 ${SH_VULKAN_ROOT_DIR}/examples/src/graphics/scene.c)
add_executable(shvulkan-headless-scene        ${SH_VULKAN_ROOT_DIR}/examples/src/graphics/headless-scene.c)

target_link_libraries(shvulkan-compute-power-numbers PUBLIC shvulkan)
target_link_libraries(shvulkan-headless-scene        PUBLIC shvulkan vvo)

if (WIN32)
target_link_libraries(shvulkan-clear-color PUBLIC shvulkan glfw)
target_link_libraries(shvulkan-scene       PUBLIC shvulkan glfw)
elseif (UNIX)
target_link_libraries(shvulkan-clear-color    PUBLIC shvulkan glfw X11 m)
target_link_libraries(shvulkan-scene          PUBLIC shvulkan glfw X11 m)
target_link_libraries(shvulkan-headless-scene PUBLIC m)
endif(WIN32)

set_target_properties(
    shvulkan-compute-power-numbers 
    shvulkan-clear-color 
    shvulkan-scene
    shvulkan-headless-scene

    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY      ${SH_VULKAN_BIN_DIR}
    VS_DEBUGGER_WORKING_DIRECTORY ${SH_VULKAN_BIN_DIR}
)

endfunction()