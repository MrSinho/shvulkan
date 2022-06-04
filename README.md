
# shvulkan

## [Build status](https://github.com/MrSinho/ShVulkan/tree/main/.ShCI)

[![linux_badge](.shci/linux-status.svg)](https://github.com/MrSinho/ShVulkan/tree/main/.shci/linux-log.md)
[![windows_badge](.shci/windows-status.svg)](https://github.com/MrSinho/ShVulkan/tree/main/.shci/windows-log.md)

# [Online documentation](https://mrsinho.github.io/docs/shvulkan/index)

`shvulkan` is a lightweight and flexible wrapper around the Vulkan API written completely in C. Vulkan is known for not being beginner friendly, and many operations may become repetitive if you have multiple projects, and that is where shvulkan becomes useful. It isn’t invasive like a graphics engine (for that take a look at shengine) but it may become a key tool if you want to work with efficient graphics without writing thousands of lines of code.

## Build guide

```bash
git clone https://github.com/MrSinho/shvulkan
cd shvulkan
mkdir build 
cd build
cmake ..
```

### Vulkan version
> Vulkan 1.3 updates are coming soon

If your machine does not support Vulkan 1.2 you can set the cmake options `SH_USE_VULKAN_1_0` or `SH_USE_VULKAN_1_1` as true:
```bash
cmake -DSH_USE_VULKAN_1_0=ON ..
```

## Features
* Vulkan instance and validation layers setup
* Physical device selection and logical device creation
* Graphics and compute queues, graphics and compute command buffers
* Depth buffer support with single setup call
* Automatic support for `VK_LAYER_KRHONOS_validation` `VK_EXT_memory_budget`, `VK_KHR_swapchain`
* Graphics and compute pipeline support, with examples
* Easier static and dynamic descriptor sets creation

## To do
* Multithreading
* Textures support
* SPIRV-Reflect integration

## Examples

The [examples](https://github.com/MrSinho/ShVulkan/blob/main/examples/src) can be built by setting on the `SH_VULKAN_BUILD_EXAMPLES` cmake option. The example summarizes almost all the available features of the library, such as setting up the prerequisites, writing buffers, push constants, static and dynamic uniform buffers, creating graphics pipelines and rendering.

### More concrete examples
* [SH-Engine](https://github.com/MrSinho/SH-Engine)
* [Gaia_Universe_Model](https://github.com/MrSinho/Gaia_Universe_Model)