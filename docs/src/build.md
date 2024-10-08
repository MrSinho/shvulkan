# Build shvulkan

![](https://img.shields.io/badge/shvulkan%20|%20SINHO%20SOFTWORKS-3CFADC?style=for-the-badge&logo=&logoColor=white&labelColor=990042)
[![](https://img.shields.io/badge/GitHub_repository-3CFADC?style=for-the-badge&logo=github&logoColor=black)](https://github.com/mrsinho/shvulkan)

[TOC]

## Clone and Build

Open the terminal and run the following commands:

```bash
git clone --recursive https://github.com/mrsinho/shvulkan.git
cd shvulkan
mkdir build
cd build
cmake -DSH_VULKAN_BUILD_EXAMPLES=ON ..
cmake --build .
```

---

## CMake targets and variables

| CMake Target                   | Type            | Configure Flags             |
|--------------------------------|-----------------|-----------------------------|
| shvulkan                       | library         | /                           |
| shvulkan-docs                  | Doxygen outputs | /                           |
| shvulkan-clear-color           | executable      | SH_VULKAN_BUILD_EXAMPLES=ON |
| shvulkan-scene                 | executable      | SH_VULKAN_BUILD_EXAMPLES=ON |
| shvulkan-compute-example       | executable      | SH_VULKAN_BUILD_EXAMPLES=ON |

If the cmake option `SH_VULKAN_BUILD_EXAMPLES` is enabled, the additional [`glfw`](https://github.com/glfw/glfw) target will be generated as a static library.

| CMake Variable                 | About                                                  |
|--------------------------------|--------------------------------------------------------|
| SH_VULKAN_VERSION              | Version of the `shvulkan` library                      |
| SH_VULKAN_ROOT_DIR             | Absolute path to the root of the repository directory  |
| SH_VULKAN_BINARIES_DIR         | Absolute path to the output binaries directory         |

## Embed shvulkan with your C/C++ projects

To link to the `shvulkan` library with CMake:

```cmake
if (NOT TARGET shvulkan)

set(SH_VULKAN_ROOT_DIR     path/to/shvulkan/root/directory)
set(SH_VULKAN_BINARIES_DIR path/your/binaries/directory)

include(${SH_VULKAN_ROOT_DIR}/shvulkan/shvulkan.cmake)
build_shvulkan()

endif()

# [...]

target_link_libraries(app PUBLIC shvulkan)

```

Then, include the `shVulkan.h` header file:

```c
#include <shvulkan/shVulkan.h">
```