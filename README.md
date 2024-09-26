# shvulkan

![](https://img.shields.io/badge/shvulkan%20|%20SINHO%20SOFTWORKS-3CFADC?style=for-the-badge&logo=&logoColor=white&labelColor=990042)
[![](https://img.shields.io/badge/GitHub_repository-3CFADC?style=for-the-badge&logo=github&logoColor=black)](https://github.com/mrsinho/shvulkan)

[TOC]

`shvulkan` is a lightweight and flexible wrapper around the Vulkan API written completely in C, that makes it easier to work with graphics efficiently without writing thousands of lines of code.

![](./docs/media/alpha-blending.png)

__Scene example__: *alfa blending, instancing and indexed draw calls example.*

![](./docs/media/scene-headless-overview.png)
__Headless scene example using [vulkan-virtual-outputs](https://github.com/mrsinho/vulkan-virtual-outputs)__: *alfa blending, instancing and indexed draw calls also here, but the image is streamed in an http server.*

---

## Build status

![](https://img.shields.io/badge/Written_in_C-3CFADC?style=for-the-badge&logo=c&logoColor=white&labelColor=0AD0DF#.svg)
![](https://img.shields.io/badge/Compatible_with_C%2b%2b-3CFADC?style=for-the-badge&logo=c%2b%2b&logoColor=white&labelColor=0AD0DF#.svg)

The examples are frequently being tested on Windows 11, Linux Mint (virtual machine and pc) with different compilers (MSVC, gcc), and hardware configurations (RX580 4GB GDDR5, Radeon V Carrizo 500MB).

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

---

## Examples

`shvulkan` ships with one [`compute example`](https://github.com/mrsinho/shvulkan/tree/main/examples/src/compute/power-numbers.c), one graphics [`clear color example`](https://github.com/mrsinho/shvulkan/tree/main/examples/src/graphics/clear-color.c), one graphics [`scene example`](https://github.com/mrsinho/shvulkan/tree/main/examples/src/graphics/scene.c) 