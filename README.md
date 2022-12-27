# shvulkan

`shvulkan` is a lightweight and flexible wrapper around the Vulkan API written completely in C. Vulkan is known for not being beginner friendly, and many operations may become repetitive if you have multiple projects, and that is where shvulkan becomes useful. It isn’t as invasive as a graphics engine (for that take a look at shengine) but it may become a key tool if you want to work with graphics efficiently without writing thousands of lines of code.

---

## [Build status](https://github.com/MrSinho/ShVulkan/tree/main/.shci) 

[![linux_badge](.shci/linux-status.svg)](https://github.com/MrSinho/ShVulkan/tree/main/.shci/linux-log.md) [![windows_badge](.shci/windows-status.svg)](https://github.com/MrSinho/ShVulkan/tree/main/.shci/windows-log.md)

---

## Programs that use shvulkan:
* [shengine](https://github.com/mrsinho/shengine)
* [shgui](https://github.com/mrsinho/shgui)
* [gaia-universe-model](https://github.com/mrsinho/gaia-universe-model)
* many more are coming!

---

# Index
* [Clone and build](#clone-and-build)
* [CMake targets](#cmake-targets)
* [Graphics example overview](#graphics-example-overview)
* [Compute example overview](#compute-example-overview)

---

## Clone and Build
Open the terminal and run the following commands:
```bash
git clone --recursive https://github.com/MrSinho/shvulkan.git
cd shvulkan
mkdir build
cd build
cmake -DSH_VULKAN_BUILD_EXAMPLES=ON ..
cmake --build .
```

If you are having some trouble with building the targets and compiling the code, check the [Linux](https://github.com/MrSinho/shvulkan/blob/main/.shci/linux-log.md) and [Windows](https://github.com/MrSinho/shvulkan/blob/main/.shci/windows-log.md) build [logs](https://github.com/MrSinho/shvulkan/blob/main/.shci).

[top](#shvulkan)

---

### Vulkan version
> Vulkan 1.3 updates are coming soon

If you haven't installed Vulkan 1.2 you can set the cmake options `SH_USE_VULKAN_1_0` or `SH_USE_VULKAN_1_1` as true:
```bash
cmake -DSH_USE_VULKAN_1_0=ON ..
cmake -DSH_USE_VULKAN_1_1=ON ..
```

[top](#shvulkan)

---

## CMake targets

| CMake target                                           | type       |
|--------------------------------------------------------|------------|
| [shvulkan](../ShVulkan/index)                       	 | library    |
| [shvulkan-graphics-example](#graphics-example)         | executable |
| [shvulkan-compute-example](#compute-example)           | executable |

If the cmake option `SH_VULKAN_BUILD_EXAMPLES` is enabled, the additional [`glfw`](https://github.com/glfw/glfw) target will be generated as a static library.

[top](#shvulkan)

---

## [Graphics example overview](examples/src/graphicsExample.c)

[![](examples/diagrams/shvulkan%20graphics.drawio.svg)](examples/diagrams/shvulkan%20graphics.drawio.svg)

[top](#shvulkan)

---

## [Compute example overview](examples/src/computeExample.c)

[![](examples/diagrams/shvulkan%20compute.drawio.svg)](examples/diagrams/shvulkan%20compute.drawio.svg)

[top](#shvulkan)
 
---
